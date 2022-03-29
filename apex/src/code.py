from stateMachine import diagnostic
from speaker import Speaker 
from sensors import Sensors

# speaker = Speaker()
sensors = Sensors()
# state = diagnostic(speaker, sensors) 
# try: 
#     while (state is not None):
#        state = state.run()
# except Exception as e:
#     file = open("error.txt", "w")
#     file.write(str(e))
#     file.close() 
#     raise e

for i in range(10):
    print(sensors._getBarometer())