import customtkinter as ctk
import serial
import sys
import glob
import threading
import sounddevice as sd
import time as ti
import threading
import math
global genvolume
genvolume=0
demo=False
port="COM3"
port2="COM4"
def optionmenu_callback1(choice):
    global port
    port=choice
def optionmenu_callback2(choice2):
    global port2
    port2=choice2
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

def demo_mode():
    global demo
    demo=True
    setup.destroy()

demobtn=ctk.CTkButton(master=setup, text="Demo", command=demo_mode) 
demobtn.pack(padx=20, pady=10)   
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


if demo==False:
    ser = serial.Serial(port, 115200)
    ser2 = serial.Serial(port2, 115200)


def printline(volume):
    volume=int(math.sqrt(volume)/2)
    if volume>50:
        volume=50
    if volume<0:
        volume=0
    update_height(volume)
    

def audio_callback(indata, frames, time, status):
    sum=0
    # Questa funzione viene chiamata per ogni blocco di audio catturato
    # Puoi elaborare 'indata' qui
    volume=indata*100
    for i in range(len(volume)):
        sum=sum+volume[i]
    volume=sum/len(volume)
    if sum>0:
        vol=int(sum[0])
    else:
        vol=(int(sum[0])*(-1))
    
    printline(vol)
    #sleep for 0.01 seconds
    ti.sleep(0.01)
    
    
def start():
# Get the name of the audio output interface
    devices = sd.query_devices()
    output_interface = None
    for device in devices:
        if device['max_output_channels'] > 0:
            output_interface = device['name']
            break
    output_interface = "Line 1 (Virtual Audio Cable), MME"
    if output_interface is None:
        raise Exception("No audio output interface found.")
    else:
    # Imposta la callback per l'acquisizione audio
        stream = sd.InputStream(device=output_interface, channels=1, callback=audio_callback, samplerate=44100)

    # Avvia l'acquisizione audio
        with stream:
            sd.sleep(10000000)  # Durata dell'acquisizione audio in millisecondi


threading.Thread(target=start).start()

def update_height(volume):
    global genvolume
    if volume<genvolume:
        volume=genvolume-6
        if volume<0:
            volume=0
    genvolume=volume
    bar1.set(volume/50)
    if (demo==False):
        ser.write(str(volume).encode())
        ser.write("p".encode())
    bar2.set(volume/50)
    if (demo==False):
        ser2.write(str(volume).encode())
        ser2.write("p".encode())
    bar1.update()
    bar2.update()
    

def send_char():
    
        if radiobutton_var.get() == 'r':
            bar1.configure(progress_color='red')
        elif radiobutton_var.get() == 'g':
            bar1.configure(progress_color='green')
        elif radiobutton_var.get() == 'b':
            bar1.configure(progress_color='blue')
        elif radiobutton_var.get() == 'v':
            bar1.configure(progress_color='violet')
        elif radiobutton_var.get() == 'y':
            bar1.configure(progress_color='yellow')
        elif radiobutton_var.get() == 'c':
            bar1.configure(progress_color='cyan')
        elif radiobutton_var.get() == 'w':
            bar1.configure(progress_color='white')
        elif radiobutton_var.get() == 'm':
            bar1.configure(progress_color="lime") 
        elif radiobutton_var.get() == "f" :
            bar1.configure(progress_color="White") 
        elif radiobutton_var.get() == "l" :
            bar1.configure(progress_color="Lime")
        else:
            raise Exception("Error")
        if (demo==False):
            ser.write((radiobutton_var.get()+"p").encode())
def send_char2():
    
    if radiobutton_var2.get() == 'r':
        bar2.configure(progress_color='red')
    elif radiobutton_var2.get() == 'g':
        bar2.configure(progress_color='green')
    elif radiobutton_var2.get() == 'b':
        bar2.configure(progress_color='blue')
    elif radiobutton_var2.get() == 'v':
        bar2.configure(progress_color='violet')
    elif radiobutton_var2.get() == 'y':
        bar2.configure(progress_color='yellow')
    elif radiobutton_var2.get() == 'c':
        bar2.configure(progress_color='cyan')
    elif radiobutton_var2.get() == 'w':
        bar2.configure(progress_color='white')
    elif radiobutton_var2.get() == 'm':
        bar2.configure(progress_color="lime") 
    elif radiobutton_var2.get() == "f" :
        bar2.configure(progress_color="White") 
    elif radiobutton_var2.get() == "l" :
        bar2.configure(progress_color="Lime")
    else:
        raise Exception("Error")
    if (demo==False):
        ser2.write((radiobutton_var2.get()+"p").encode())
        
root = ctk.CTk()
root.title("Controller")
root.geometry("450x600")
radiobutton_var = ctk.StringVar()
radiobutton_var.set('l')
radiobutton_var2 = ctk.StringVar()
radiobutton_var2.set('l')
# Create two vertical bars
bar1 = ctk.CTkProgressBar(root, width=40, orientation='vertical', height=550, progress_color="lime")
bar2 = ctk.CTkProgressBar(root, width=40, orientation='vertical', height=550, progress_color="lime")

# Pack the bars to the right side of the window
bar1.place(relx=0.5, rely=0.95, anchor='s')
bar2.place(relx=0.7, rely=0.95, anchor='s')
label=ctk.CTkLabel(master=root, text="Select Effect to 1", font=("Arial", 20))
label.pack(anchor='w', pady=10, padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"Red", variable=radiobutton_var, value='r', command=send_char)
radiobutton.pack(anchor='w',padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"Green", variable=radiobutton_var, value='g', command=send_char)
radiobutton.pack(anchor='w', padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"Blue", variable=radiobutton_var, value='b', command=send_char)
radiobutton.pack(anchor='w', padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"Violet", variable=radiobutton_var, value='v', command=send_char)
radiobutton.pack(anchor='w', padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"Yellow", variable=radiobutton_var, value='y', command=send_char)
radiobutton.pack(anchor='w', padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"Cyan", variable=radiobutton_var, value='c', command=send_char)
radiobutton.pack(anchor='w', padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"White", variable=radiobutton_var, value='w', command=send_char)
radiobutton.pack(anchor='w', padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"Rainbow", variable=radiobutton_var, value='m', command=send_char)
radiobutton.pack(anchor='w', padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"full White", variable=radiobutton_var, value='f', command=send_char)
radiobutton.pack(anchor='w', padx=20)
radiobutton = ctk.CTkRadioButton(root, text=f"full Rainbow", variable=radiobutton_var, value='l', command=send_char)
radiobutton.pack(anchor='w', padx=20)
label = ctk.CTkLabel(master=root, text="Select Effect to 2", font=("Arial", 20))
label.pack(anchor='w', pady=10, padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"Red", variable=radiobutton_var2, value='r', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"Green", variable=radiobutton_var2, value='g', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"Blue", variable=radiobutton_var2, value='b', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"Violet", variable=radiobutton_var2, value='v', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"Yellow", variable=radiobutton_var2, value='y', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"Cyan", variable=radiobutton_var2, value='c', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"White", variable=radiobutton_var2, value='w', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"Rainbow", variable=radiobutton_var2, value='m', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"full White", variable=radiobutton_var2, value='f', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
radiobutton2 = ctk.CTkRadioButton(root, text=f"full Rainbow", variable=radiobutton_var2, value='l', command=send_char2)
radiobutton2.pack(anchor='w', padx=20)
root.mainloop()
