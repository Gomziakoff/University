def create_keyboard():
    root = Tk()
    root.title("Виртуальная клавиатура")
   
    # Первый ряд
    frame1 = Frame(root)
    frame1.pack()
    keys1 = ['`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'Backspace']
    for key in keys1:
        Button(frame1, text=key, width=4).pack(side=LEFT)
   
    # Второй ряд
    frame2 = Frame(root)
    frame2.pack()
    keys2 = ['Tab', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\\']
    for key in keys2:
        Button(frame2, text=key, width=4).pack(side=LEFT)
   
    # Третий ряд
    frame3 = Frame(root)
    frame3.pack()
    keys3 = ['Caps', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', "'", 'Enter']
    for key in keys3:
        Button(frame3, text=key, width=4).pack(side=LEFT)
   
    # Четвертый ряд
    frame4 = Frame(root)
    frame4.pack()
    keys4 = ['Shift', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 'Shift']
    for key in keys4:
        Button(frame4, text=key, width=4).pack(side=LEFT)
   
    # Пятый ряд
    frame5 = Frame(root)
    frame5.pack()
    Button(frame5, text='Ctrl', width=4).pack(side=LEFT)
    Button(frame5, text='Space', width=20).pack(side=LEFT)
    Button(frame5, text='Alt', width=4).pack(side=LEFT)
   
    root.mainloop()

create_keyboard()
