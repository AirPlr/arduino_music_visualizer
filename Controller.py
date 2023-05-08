import customtkinter as ctk
import serial

import sys
import glob
import serial

port='COM3'
port2='COM4'
def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


setup=ctk.CTk()
setup.title("Setup")
setup.geometry("300x250")
def optionmenu_callback1(choice):
    port1=choice
def optionmenu_callback2(choice2):
    port2=choice2
    
label = ctk.CTkLabel(master=setup, text="Select Port 1")
label.pack(padx=20, pady=10)
combobox = ctk.CTkOptionMenu(master=setup,
                                       values=serial_ports(),
                                       command=optionmenu_callback1
                                    )
combobox.pack(padx=20, pady=10)  # set initial value
label = ctk.CTkLabel(master=setup, text="Select Port 2")
label.pack(padx=20, pady=10)
combobox = ctk.CTkOptionMenu(master=setup,
                                       values=serial_ports(),
                                       command=optionmenu_callback2)
combobox.pack(padx=20, pady=10)  # set initial value
setup.mainloop()



ser = serial.Serial(port, 115200)
ser2 = serial.Serial(port2, 115200)

def send_char():
    
    ser.write(radiobutton_var.get().encode())
    if movetoghether.get():
        radiobutton_var2.set(radiobutton_var.get())
        ser2.write(radiobutton_var.get().encode())
    
def send_char2():
    
    ser2.write(radiobutton_var.get().encode())
    if movetoghether.get():
        radiobutton_var.set(radiobutton_var.get())
        ser.write(radiobutton_var.get().encode())
        

root = ctk.CTk()
root.title("Controller")
root.geometry("300x600")
radiobutton_var = ctk.StringVar()
radiobutton_var.set('l')
radiobutton_var2 = ctk.StringVar()
radiobutton_var2.set('l')
movetoghether = ctk.BooleanVar()
movetoghether.set(True)
movetoghetherbutton = ctk.CTkCheckBox(root, text="Move toghether", variable=movetoghether)
movetoghetherbutton.pack(padx=20, pady=10)
label=ctk.CTkLabel(master=root, text="Select Effect to 1")
label.pack(padx=20, pady=10)
radiobutton = ctk.CTkRadioButton(root, text=f"Red", variable=radiobutton_var, value='r', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"Green", variable=radiobutton_var, value='g', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"Blue", variable=radiobutton_var, value='b', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"Violet", variable=radiobutton_var, value='v', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"Yellow", variable=radiobutton_var, value='y', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"Cyan", variable=radiobutton_var, value='c', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"White", variable=radiobutton_var, value='w', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"Rainbow", variable=radiobutton_var, value='m', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"full White", variable=radiobutton_var, value='f', command=send_char)
radiobutton.pack()
radiobutton = ctk.CTkRadioButton(root, text=f"full Rainbow", variable=radiobutton_var, value='l', command=send_char)
radiobutton.pack()
label = ctk.CTkLabel(master=root, text="Select Effect to 2")
label.pack(padx=20, pady=10)
radiobutton2 = ctk.CTkRadioButton(root, text=f"Red", variable=radiobutton_var2, value='r', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"Green", variable=radiobutton_var2, value='g', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"Blue", variable=radiobutton_var2, value='b', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"Violet", variable=radiobutton_var2, value='v', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"Yellow", variable=radiobutton_var2, value='y', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"Cyan", variable=radiobutton_var2, value='c', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"White", variable=radiobutton_var2, value='w', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"Rainbow", variable=radiobutton_var2, value='m', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"full White", variable=radiobutton_var2, value='f', command=send_char2)
radiobutton2.pack()
radiobutton2 = ctk.CTkRadioButton(root, text=f"full Rainbow", variable=radiobutton_var2, value='l', command=send_char2)
radiobutton2.pack()

root.mainloop()