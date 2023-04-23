import tkinter as tk
import tkinter.messagebox
import serial.tools.list_ports
import binascii
import sys
import matplotlib.pyplot as plt

import matplotlib.animation as animation
from matplotlib import style
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

import random
from random import randrange

#TODO add plotting function
#TODO get rid of excessive buttons for now
#TODO separete to files




import numpy as np

x = np.linspace(0, 2 * np.pi, 200)
y = np.sin(x)




selected_port='COM1'
error_occured=True #for handling users clicking on 'X' in the window bar.
#executed when user choosed port. It destroys dialog window, and creates main window
def select_port(event=None):
    global selected_port
    global error_occured
    print(listbox.curselection())
    selected_port = str(ports[listbox.curselection()[0]].device)
    error_occured=False
    root.destroy()


# get available serial ports
ports = serial.tools.list_ports.comports()
# print([port.description for port in ports])

# create GUI of the dialog box
root = tk.Tk()
root.title('Choose COM port')
listbox = tk.Listbox(root, width=100, height=0)
listbox.insert(1, *ports)
listbox.pack(padx=10, pady=20, side=tk.TOP)
button = tk.Button(root, text="Confirm", command=select_port)
button.pack(pady=5, side=tk.TOP)

listbox.focus_set()
listbox.activate(0)
listbox.select_set(0)
root.bind('<Return>', select_port)
root.mainloop()  ###

if error_occured == True:
    sys.exit()

##############################################################
ser = serial.Serial(
    port=selected_port,
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

print('asdasd')
print(selected_port)

############################################################
# GUI creation (2nd window) and keys binding
main_window = tk.Tk()
main_window.title('PC_visualizer')
axis_1 = tk.Scale(main_window,
                  from_=200,
                  to=0,
                  orient=tk.VERTICAL,
                  showvalue=True,
                  label='Axis 1')
axis_1.set(100)
axis_1.pack(padx=100, side=tk.BOTTOM)


#main_window.bind('<a>', inc_axis_1)

main_window.focus_force()
main_window.update()  # if not called before while, it can wait 1 sec, and becomes non responsive

##############################################################
xar = []
yar = []

style.use('ggplot')
fig = plt.figure(figsize=(14, 4.5), dpi=100)
ax1 = fig.add_subplot(1, 1, 1)
ax1.set_ylim(0, 100)
ax1.set_xlim(0, 101)
line, = ax1.plot(xar, yar, 'r', marker='o')
def animate(i):
    yar.append(randrange(0,100))
    xar.append(i)
    line.set_data(xar, yar)
    ax1.set_xlim(0, i+1)

plotcanvas = FigureCanvasTkAgg(fig, main_window)
plotcanvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)
# ani = animation.FuncAnimation(fig, animate, interval=1000, blit=False)
#
# root.mainloop()

##############################################################
from time import sleep
# main loop
while True:
    sleep(1)
    X = np.linspace(0, 100,101)
    Y = np.random.rand(1,101)*100
    line.set_data(X,Y)
    fig.canvas.draw()
    fig.canvas.flush_events()
    """try:    
        rcv=ser.read(20)
    except:
        tkinter.messagebox.showinfo(
            title="Error",
            message="Cannot receive data. Port malfunction! Shutting down app!")
        sys.exit()
"""
    main_window.update()

