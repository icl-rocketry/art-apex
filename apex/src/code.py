from state_machine import diagnostic

state = diagnostic()

try:
    while state is not None:
        state = state.run()
except Exception as e:
    import os
    os.rename("boot.py", "boo.py")
    print("Renamed")
    file = open("error.txt", "w")
    file.write(str(e))
    file.close()