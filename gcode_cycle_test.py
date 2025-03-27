import time
from communication.serial_comm import SerialCommunication
from vision.color_detection import ColorDetector  # Импортируем модуль ColorDetector

# Инициализация последовательного соединения
ser = SerialCommunication()
print("Доступные порты:", ser.list_ports())
port = input("Введите порт для подключения: ")
ser.connect(port, 115200)

# Инициализация детектора цвета
color_detector = ColorDetector(1)

def wait_ack(cmd_ID=None):
    while True:
        response = ser.read_response()
        tokens = response.split()
        if not cmd_ID:
            if len(tokens) >= 2 and tokens[0] == '[ACK]':
                if tokens[1] != 'OK':
                    print(response)
                break
        else:
            if len(tokens) >= 3 and tokens[0] == '[ACK]' and tokens[1] == cmd_ID:
                if tokens[2] != 'OK':
                    print(response)
                break

def sendCmd(cmd):
    ser.send_command(cmd)

def sendCmd_waitACK(cmd, cmdId=None):
    ser.send_command(cmd)
    #time.sleep(1)
    wait_ack(cmdId)

def home(u=True, l=True, c=True):
    cmd = 'G28 '
    if u: cmd += 'U'
    if l: cmd += 'L'
    if c: cmd += 'C'
    sendCmd_waitACK(cmd)

def nextCup():
    cupFound = False
    while not cupFound:
        sendCmd_waitACK('G2 C360')
        sendCmd('M13')
        response = ser.read_response()
        print(response)
        #cupFound = response[0] == '1'
        cupFound = True
        wait_ack()

upperDisk_cupCatch_pos = -70

def catchCup():
    cmd = 'G0 U' + str(upperDisk_cupCatch_pos)
    sendCmd_waitACK(cmd)

# Определенные цвета и соответствующие позиции
colors = ['unknown', 'white', 'black', 'red', 'orange', 'green', 'blue', 'yellow', 'brown', 'gold']  
#positions = [0, 60, 120, 180, 180, 240, 300, 360, 360, 360]
#posUpperDisk = [70,  110,  160,  205,  250,  295,  340,  385]
#posUpperDisk = [80,  120,  170,  215,  260,  305,  350,  395]
#posLowerDisk = [-20, -63, -108, -153, -198, -243, -288, -333]
posUpperDisk = [65,  110,  155,  200,  240,  295,  342,  390]
posLowerDisk = [-20, -63, -108, -153, -198, 117, 72, 25]
 

def sortCup(color):
    print("Захват крышечки верхним диском...")
    #time.sleep(1)
    sendCmd_waitACK("M2 S300")
    print("Крышечка захвачена.")
    if color in colors:
        index = colors.index(color)
        posU = posUpperDisk[index]
        posL = posLowerDisk[index]
        
        #Установка нижнего диска в зоне найденного цвета
        cmd = 'G0 L' + str(posL)
        sendCmd_waitACK(cmd)
        time.sleep(1)
        ##Установка верхнего диска под зоной сканирования для захвата крышечки
        #sendCmd_waitACK('G0 U-70')
        catchCup()
        time.sleep(1)
        sendCmd_waitACK("M2 S300")
        #Перемещаем верхний диск с захваченной крышечкой до зоны найденного цвета
        
        cmd = 'G0 U390'
        #cmd = 'G0 U' + str(posU)
        sendCmd_waitACK(cmd)
        sendCmd_waitACK("M2 S300")
        
    else:
        print(f"Неизвестный цвет: {color}. Пропуск сортировки.")

def check_color():
    frame = color_detector.capture_frame()
    if frame is not None:
        color = color_detector.detect_color(frame)
        return color
    else:
        print("Не удалось захватить кадр для определения цвета.")
        return 'unknown'

def cycle(num):
    #print("Возврат верхнего и нижнего дисков в домашнее положение...")
    #home(True, True, False)
    for i in range(num):
        print("Возврат верхнего диска в домашнее положение...")
        home(True, False, False)
        print("Возврат нижнего диска в домашнее положение...")
        home(False, True, False)
        print("Диски дома!")
        print("Поиск следующей крышечки...")
        nextCup()
        print("Крышечка найдена.")
        sendCmd_waitACK("M2 S300")
        color = check_color()
        print("Обнаруженный цвет: " + color)
        print("Сортировка крышечки...")
        sortCup(color)
        

sendCmd_waitACK('M40 L0')
sendCmd_waitACK('M40 L0')
try:
    while True:
        userCmd = input("Введите команду: ").split()
        if len(userCmd):
            if userCmd[0] == 'S':
                count = 1
                if len(userCmd) >= 2:
                    count = int(userCmd[1])
                cycle(count)
except KeyboardInterrupt:
    print("Программа остановлена пользователем.")
finally:
    ser.disconnect()
    color_detector.release()  # Освобождаем ресурсы камеры
    print("Ресурсы освобождены. Программа завершена.")
