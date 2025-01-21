import numpy as np

from CampusEnv import CampusEnv
from typing import List, Tuple
import heapdict
import time
from IPython.display import clear_output

class Node:
    def __init__(self, state, parent=None, action=None, cost=0, g=0, h=0, f=0, h_weight=0):
        self.state = state
        self.parent = parent
        self.action = action
        self.cost = cost
        self.g = g
        self.h = h
        self.f = f
        self.h_weight = h_weight

    def __lt__(self, other):
        return (self.f, self.state) < (other.f, other.state)

    def __eq__(self, other):
        return (self.f, self.state) == (other.f, other.state)

    def __hash__(self):
        return hash((self.state, self.f))
    
    
#     def calculate_f(self, env: CampusEnv):
#         goal_states = env.get_goal_states()
#         h_set = set()
#         for goal in goal_states:
#             rows = abs(env.to_row_col(goal)[0] - env.to_row_col(self.state)[0])
#             cols = abs(env.to_row_col(goal)[1] - env.to_row_col(self.state)[1])
#             h_set.add(rows + cols)
#         self.h = min(min(h_set), 100)
#         self.f = self.h_weight * self.h + (1 - self.h_weight) * self.g
            

    
# def g_compare(node: Node):        #TODO we need compare for g and for f
#         return node.g

# def f_compare(node: Node):
#         return node.f

    
class Agent:
    def __init__(self):
        self.env = None
        self.current_state = None
        self.goal_states = None
        self.expanded = 0

    def solution_wrapper(self, curr_node):
        actions, cost = self.solution(curr_node, [], 0)
        return actions, int(cost)  # Ensure the cost is returned as an integer
    
    def solution(self, curr_node :Node, actions :list, cost :int)-> Tuple[List[int], int]:
        if curr_node is None:
            return actions, cost
        if curr_node.parent is None:
            return actions, cost
        self.env.set_state(curr_node.parent.state)
        state, step_cost, ter = self.env.step(curr_node.action)
        actions.insert(0,curr_node.action)
        return self.solution(curr_node.parent, actions, cost + step_cost)
    
    def animation(self, epochs: int ,state: int, action: List[int], total_cost: int) -> None:
        clear_output(wait=True)
        print(self.env.render())
        print(f"Timestep: {epochs}")
        print(f"State: {state}")
        print(f"Action: {action}")
        print(f"Total Cost: {total_cost}")
        time.sleep(1)


    
    
  
#################################################################################################################################
#################################################################################################################################




class DFSGAgent(Agent):
    def __init__(self) -> None:
        super().__init__()
 

    def search(self, env: CampusEnv) -> Tuple[List[int], float, int]:
        self.env = env
        self.env.reset()
        self.expanded=0
        open = []
        closed: set[int] = set()
        curr_node = Node(self.env.get_state())
        open.append(curr_node)
        goal_node = self.recursiveDFSG(open,closed)
        return self.solution_wrapper(goal_node) + (self.expanded,)

    def recursiveDFSG(self, open :List[Node], closed :set[int]) -> Node:
        curr_node = open.pop()
        closed.add(curr_node.state)
        #self.animation(self.expanded,curr_node.state,curr_node.action,curr_node.cost)
        if(self.env.is_final_state(curr_node.state)):
            return curr_node
        self.expanded += 1
        if None in self.env.succ(curr_node.state)[0]:   #current_node is a hole
            return None
        for action, child_info in self.env.succ(curr_node.state).items():
            self.env.set_state(curr_node.state)
            child_node = Node(child_info[0], curr_node, action, child_info[1])
            states_list = [s.state for s in open]
            if child_node.state in closed or child_node.state in states_list:
                continue
            else:
                open.append(child_node)
                goal_node = self.recursiveDFSG(open, closed)
                if goal_node is not None:
                    return goal_node    
        return None














class UCSAgent(Agent):
  
    def __init__(self) -> None:
        super().__init__()


    def search(self, env: CampusEnv) -> Tuple[List[int], float, int]:
        goal_node = self.searchUCS(env)
        return self.solution_wrapper(goal_node) + (self.expanded,)
    
    def searchUCS(self, env: CampusEnv) -> Node:
            self.env = env
            self.env.reset()
            self.expanded=0
            #key: Node, Values: g
            open = heapdict.heapdict()
            closed = set()
            curr_node: Node = Node(self.env.get_state())
            open[curr_node] = (0,0) 
            
            while open:     #while open is not empty
                #self.animation(self.expanded,curr_node.state,curr_node.action,curr_node.cost)
                curr_node = open.popitem()[0]
                closed.add(curr_node.state)
                if(self.env.is_final_state(curr_node.state)):
                    return curr_node
                self.expanded += 1
                
                #print(self.env.succ(curr_node.state).items())
                for action, child_info in self.env.succ(curr_node.state).items():
                    if child_info[1] == None:
                        continue
                    #self.env.set_state(curr_node.state)     #TODO why do we have this?
                    child_node = Node(child_info[0], curr_node, action, child_info[1], child_info[1] + curr_node.g)
                    states_list = [s[1] for s in open.values()]
                    if (child_node.state not in closed) and (child_node.state not in states_list): #maybe remove .keys?
                        open[child_node] = (child_node.g, child_node.state)
                    elif child_node.state in states_list:
                        to_delete = []
                        for node, val in open.items():
                            if val[0] > child_node.g and val[1] == child_node.state:
                                to_delete.append(node)
                        if to_delete:
                            del open[to_delete[0]]
                            open[child_node] = (child_node.g, child_node.state)
            return None











import math

class WeightedAStarAgent(Agent):
    
    def h_func(self, state): 
        x_state, y_state = self.env.to_row_col(state)
        goal_states = self.env.get_goal_states()
        goal_manhattan_set = set()
        for goal in goal_states:
            x_goal, y_goal = self.env.to_row_col(goal)
            manhattan_distance = abs(x_goal-x_state) + abs(y_goal- y_state)
            goal_manhattan_set.add(manhattan_distance)
        return min(100, min(goal_manhattan_set))
    
    def f_func(self, h, g ,h_weight):              
        new_f_score = (h_weight*h) + ((1-h_weight)*g)
        return new_f_score
    
    def __init__(self):
        super().__init__()

    def search(self, env: CampusEnv, h_weight) -> Tuple[List[int], float, int]:
        goal_node = self.searcWAstar(env, h_weight)
        return self.solution_wrapper(goal_node) + (self.expanded,)  
    
    def searcWAstar(self, env: CampusEnv, h_weight) -> Node:
        self.env = env
        self.env.reset()
        self.expanded=0
        
        open = heapdict.heapdict()
        closed = set()
        open_states_set = set()
        closed_states_set = set()
        
        new_h = self.h_func(self.env.get_state())
        new_f_score = self.f_func(new_h, 0, h_weight)
        curr_node: Node = Node(self.env.get_state(), h=new_h, f=new_f_score)
        #curr_node.calculate_f(env)      #calculating f using env
        
        
        open[curr_node] = (curr_node.f, 0) #open heapdict: key=node, vals=f score, state
        open_states_set.add(curr_node.state)
        
        while open:     #while open is not empty
            curr_node = open.popitem()[0]
            open_states_set.remove(curr_node.state)
            closed.add(curr_node)
            closed_states_set.add(curr_node.state)
            
            if(self.env.is_final_state(curr_node.state)):
                return curr_node
            self.expanded += 1
            #print(self.env.succ(curr_node.state).items())
            
            for action, child_info in self.env.succ(curr_node.state).items():
                if child_info[1] == None:
                        continue
                child_g = curr_node.g + child_info[1]
                child_h = self.h_func(child_info[0])
                child_f_score = self.f_func(child_h, child_g, h_weight)
                child_node = Node(child_info[0], curr_node, action, child_info[1], child_g, child_h, child_f_score, h_weight)
                
                if (child_node.state not in closed_states_set) and (child_node.state not in open_states_set): 
                    open[child_node] = (child_node.f, child_node.state)
                    open_states_set.add(child_node.state)
                    
                elif child_node.state in open_states_set:
                    for node, val in open.items():
                        if val[1] == child_node.state and val[0] > child_node.f: 
                            del open[node]
                            # open_states_set.remove(node.state)
                            open[child_node] = (child_node.f, child_node.state)
                            # open_states_set.add(child_node.state)
                            break
                        
                elif child_node.state in closed_states_set:
                    for node in closed:
                        if node.state == child_node.state and node.f > child_node.f:
                            closed.remove(node)
                            closed_states_set.remove(node.state)
                            open[child_node] = (child_node.f, child_node.state)
                            open_states_set.add(child_node.state)
                            break
                    
                    
        return None












class AStarAgent():
    
    def __init__(self):
        super().__init__()

    def search(self, env: CampusEnv) -> Tuple[List[int], float, int]:
        agent = WeightedAStarAgent()
        return agent.search(env, 0.5)

