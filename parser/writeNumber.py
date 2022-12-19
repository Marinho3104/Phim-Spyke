

file = open("./token_definitions.h", 'r')

info = file.read()

file.close()

info = info.split('\n')

c = 1

ch = 1

for i in range (len(info)):

    if info[i] == '': 
        print("\n")
        continue

    cont = 0

    for _ in range (len(info[i])): 

        if cont == 2: 

            copy = info[i]
            
            info[i] = info[i][:_]

            info[i] += str(c)

            info[i] += " " + copy[_+1:]

            c+=1

            print(info[i])
        
            break

        elif info[i][_] == ' ': cont+=1


