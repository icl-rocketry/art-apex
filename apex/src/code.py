from state_machine import diagnostic

state = diagnostic()

while state is not None:
    state = state.run()
