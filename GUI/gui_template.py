import tkinter as tk
from tkinter import Entry
from tkinter import Canvas, Label, StringVar, Text, Variable, ttk, messagebox, Entry  
from PIL import ImageTk, Image
import serial
import sys
import glob
from serial.tools.list_ports import comports

def binaryToDecimal(binary):
 
    decimal, i = 0, 0
    while(binary != 0):
        dec = binary % 10
        decimal = decimal + dec * pow(2, i)
        binary = binary//10
        i += 1
    print(decimal)

class App(tk.Tk):

    def __init__(self):
        super().__init__()
        self.title('GUI_SEP')
        

        # layout on the root window
        self.columnconfigure(0, weight=4)
        self.columnconfigure(1, weight=1)

        # background of the root window
        self.config(bg='white')

        self.__create_widgets()

    def __create_widgets(self):

        logo_frame = LogoFrame(self)
        logo_frame.grid(column=0, row=0, sticky='nw')
        # create the input frame
        BUTTON_frame = BUTTONframe(self, logo_frame)
        BUTTON_frame.grid(column=0, row=2, sticky='sw')
        text_frame = TextFrame(self,BUTTON_frame)
        text_frame.grid(column=0, row=1, sticky='nw')

        for widget in self.winfo_children():
            widget.grid(padx=30, pady=10)


class LogoFrame(tk.Frame):
    def __init__(self, container):
        super().__init__(container)
        self.config(bg='white')
        self.__create_widgets()

    def callback(self, event):
        print(self.menu.get())
        self.com = self.menu.get()
        self.port = serial.Serial(self.com, 9600, timeout=10)
        print(self.port.name)

    def __create_widgets(self):
        self.img = ImageTk.PhotoImage(Image.open(
            'ist.png').resize((100, 97), Image.LANCZOS))
        self.canvas = tk.Canvas(
            self, bg="white", width=500, height=100, bd=0, highlightthickness=0)
        self.canvas.pack()
        self.canvas.create_image(75, 0, image=self.img, anchor='nw')

        self.aval_ports = serial_ports()
   
        self.com = 0

        self.menu = ttk.Combobox(
            self, values=self.aval_ports)
        self.menu.set('Please choose the COM port')
        self.menu.bind("<<ComboboxSelected>>", self.callback)
        self.menu.pack(side='bottom', fill='x', pady=(20,0))

class TextFrame(tk.Frame):

    def __init__(self, container, BUTTON_frame):
        super().__init__(container)
        self.BUTTON_FRAME=BUTTON_frame
        self.config(bg='white')
        self.__create_widgets()

    def __create_widgets(self):
        global receivedbyte
        global label
        label = Label(self,background='white' ,text= f"Temperature:{receivedbyte}")
        label.config(font=("Courier", 14))
        label.pack()



class BUTTONframe(tk.Frame):
    def __init__(self, container, logo_frame):
        super().__init__(container)
        # setup the grid layout manager
        self.logo = logo_frame
        self.columnconfigure(1, weight=1)
        self.config(bg='white')
        self.__create_widgets()
        

    def button_command(self):
        global receivedbyte
        if self.logo.com == 0:
            messagebox.showwarning('Warning', 'Please select COM port')
        else:
            self.button.config(image=self.off_switch)
            self.logo.port.reset_input_buffer()
            
            self.button.config(image=self.off_switch)
            self.logo.port.write(b'1')
            #while int.from_bytes(self.logo.port.read(), "big") != 255:
            #    i=1
            h1=self.logo.port.read()
            h1int=int.from_bytes(h1,"big")
            #self.logo.port.write(b'7')

            h2=self.logo.port.read()
            h2int=int.from_bytes(h2,"big")
            
            receivedbyte=str(h1int)+','+str(h2int)
            #self.button.config(image=self.off_switch)
            label.configure(text=f"Z Accelaration: {receivedbyte} C")
            print(f"Temperature: {receivedbyte} C")


    def button_command2(self):
        global receivedbyte2
        if self.logo.com == 0:
            messagebox.showwarning('Warning', 'Please select COM port')
        else:
            self.button2.config(image=self.off_switch)
            self.logo.port.reset_input_buffer()
            
            self.button2.config(image=self.off_switch)
            self.logo.port.write(b'2')
            #while int.from_bytes(self.logo.port.read(), "big") != 255:
            #    i=1
            h3=self.logo.port.read()
            h3int=int.from_bytes(h3)
            #self.logo.port.write(b'7')            
            
            receivedbyte2=str(h3int)
            #self.button.config(image=self.off_switch)
            label.configure(text=f"Nº of Accelaration Interrupts: {receivedbyte2} C")
            print(f"Temperature: {receivedbyte2} C")           
    
    
    def button_command3(self):
        global receivedbyte3
        if self.logo.com == 0:
            messagebox.showwarning('Warning', 'Please select COM port')
        else:
            self.button3.config(image=self.off_switch)
            self.logo.port.reset_input_buffer()
            
            self.button3.config(image=self.off_switch)
            self.logo.port.write(b'3')
            #while int.from_bytes(self.logo.port.read(), "big") != 255:
            #    i=1
            h4=self.logo.port.read()
            h4int=int.from_bytes(h4)
            #self.logo.port.write(b'7')            
            
            h5=self.logo.port.read()
            h5int=int.from_bytes(h5)
            #self.logo.port.write(b'7') 

            
            receivedbyte3=str(h4int)+str(h5int)
            #self.button.config(image=self.off_switch)
            label.configure(text=f"Y Accelaration: {receivedbyte3} C")
            print(f"Temperature: {receivedbyte3} C")               
    

    def __create_widgets(self):
        self.off_switch = ImageTk.PhotoImage(Image.open(
            'button.png').resize((51, 51), Image.LANCZOS))

        # Button frame, label and button init
        self.button_frame = tk.Frame(self)
        self.button_frame.config(background='white')
        self.button_frame.grid(column=0, row=2, sticky='we', padx=(60, 0))
        self.button_label = tk.Label(self.button_frame, text='Current', background='white')
        self.button_label.pack(side='bottom')
        self.button = tk.Button(self.button_frame, background='white', bd=0,highlightthickness=0, command=self.button_command, image=self.off_switch)
        self.button.pack(side='bottom')

        self.button_frame2 = tk.Frame(self)
        self.button_frame2.config(background='white')
        self.button_frame2.grid(column=0, row=3, sticky='we', padx=(60, 0))
        self.button_label2 = tk.Label(self.button_frame, text='Power', background='white')
        self.button_label2.pack(side='bottom')
        self.button2 = tk.Button(self.button_frame, background='white', bd=0,highlightthickness=0, command=self.button_command2, image=self.off_switch)
        self.button2.pack(side='bottom')

        self.button_frame3 = tk.Frame(self)
        self.button_frame3.config(background='white')
        self.button_frame3.grid(column=0, row=4, sticky='we', padx=(60, 0))
        self.button_label3 = tk.Label(self.button_frame, text='Bus Voltage', background='white')
        self.button_label3.pack(side='bottom')
        self.button3 = tk.Button(self.button_frame, background='white', bd=0,highlightthickness=0, command=self.button_command3, image=self.off_switch)
        self.button3.pack(side='bottom')
            


class TextFrame(tk.Frame):
    def __init__(self, container, BUTTON_frame):
        super().__init__(container)
        self.BUTTON_FRAME = BUTTON_frame
        self.config(bg='white')
        self.__create_widgets()

    def __create_widgets(self):
        global receivedbyte
        global label

        label = tk.Label(self, background='white', text=f"Temperature:{receivedbyte}")
        label.config(font=("Courier", 14))
        label.pack()

        self.text_box1 = Entry(self, background='white')
        self.text_box1.pack()

        self.text_box2 = Entry(self, background='white')
        self.text_box2.pack()

        # Adicione eventos Enter para as caixas de texto
        self.text_box1.bind("<Return>", self.on_text_box1_enter)
        self.text_box2.bind("<Return>", self.on_text_box2_enter)

    def on_text_box1_enter(self, event):
        # Captura o valor da primeira caixa de texto
        value = self.text_box1.get()
        # Armazene o valor em uma variável
        global value_from_text_box1
        value_from_text_box1 = value
        
        print(f"Valor da Text Box 1: {value_from_text_box1}")

    def on_text_box2_enter(self, event):
        # Captura o valor da segunda caixa de texto
        value = self.text_box2.get()
        # Armazene o valor em uma variável
        global value_from_text_box2
        value_from_text_box2 = value.encode ("utf-8")
        print (value_from_text_box2)

        print(f"Valor da Text Box 2: {value_from_text_box2}")


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



receivedbyte=''
label=''

if __name__ == "__main__":
    app = App()
    app.mainloop()
    