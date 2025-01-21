import time
from IPython.display import clear_output

from typing import List, Tuple
from CampusEnv import CampusEnv
from Algorithms import *

DOWN = 0
RIGHT = 1
UP = 2
LEFT = 3

MAPS = {
    "4x4": ["SFFF",
            "FHFH",
            "FFFH",
            "HFFG"],
    "8x8": ["SFFFFFFF",
            "FFFFFTAL",
            "TFFHFFTF",
            "FPFFFHTF",
            "FAFHFPFF",
            "FHHFFFHF",
            "FHTFHFTL",
            "FLFHFFFG"],
}

env = CampusEnv(MAPS["8x8"])
state = env.reset()
print(env.render())


def print_solution(actions,env: CampusEnv) -> None:
    env.reset()
    total_cost = 0
    print(env.render())
    print(f"Timestep: {1}")
    print(f"State: {env.get_state()}")
    print(f"Action: {None}")
    print(f"Cost: {0}")
    time.sleep(1)

    for i, action in enumerate(actions):
      state, cost, terminated = env.step(action)
      total_cost += cost
      clear_output(wait=True)

      print(env.render())
      print(f"Timestep: {i + 2}")
      print(f"State: {state}")
      print(f"Action: {action}")
      print(f"Cost: {cost}")
      print(f"Total cost: {total_cost}")

      time.sleep(1)

      if terminated is True:
        break
      


#print(env.get_goal_states())
agent = WeightedAStarAgent()
act, cost, num = agent.search(env, 0)
#print_solution(act, env)
print(act)
print(num)
print(cost)





