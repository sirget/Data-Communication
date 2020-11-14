ready = True
while(1):
    while(ready):
        x = input('waiting for command : ')
        if x == 'start':
            print('send start to Pc2')
            ready = False
        if x == 'top':
            print('000000000000000 [x,y]')
    while(not ready):
        y = input('side of picture and degree: ').split()
        print(y)
        ready = True
