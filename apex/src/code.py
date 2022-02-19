from stateMachine import diagnostic
from speaker import Speaker 

speaker = Speaker()
state = diagnostic(speaker) 
try: 
    while (state is not None):
       state = state.run()    
except Exception as e:
    import os
    os.rename("boot.py", "boo.py")
    print("Renamed")
    file = open("error.txt", "w")
    file.write(str(e))
    file.close() 
    raise e