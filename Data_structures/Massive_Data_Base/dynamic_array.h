#ifndef WET2_DYNAMIC_ARRAY
#define WET2_DYNAMIC_ARRAY

#include "wet2util.h"
#include "player.h"
#include <iostream>

#define INITIAL_ARRAY_SIZE 10
#define LOAD_FACTOR 0.75
#define EXPAND_FACTOR 2

class node_to_player
{
public:
    PlayerNode *data = nullptr;
    node_to_player *next = nullptr;
    node_to_player(PlayerNode *player = nullptr, node_to_player *next = nullptr)
    {
        this->data = player;
        this->next = next;
    }
    ~node_to_player()
    {
        this->data = nullptr;
        this->next = nullptr;
    }
    // TODO might need a destructor
};

class dynamic_array
{
public:
    int num_elements_in_array;
    int array_size;
    node_to_player **arr;
    dynamic_array()
    {
        this->num_elements_in_array = 0;
        array_size = INITIAL_ARRAY_SIZE;
        this->arr = new node_to_player *[INITIAL_ARRAY_SIZE];
        for (int i = 0; i < INITIAL_ARRAY_SIZE; i++)
        {
            this->arr[i] = nullptr;
        }
    }
    ~dynamic_array()
    {
        for (int i = 0; i < this->array_size; i++)
        {
            node_to_player *temp = this->arr[i];
            node_to_player *node_to_delete = nullptr;
            while (temp != nullptr)
            {
                node_to_delete = temp;
                temp = temp->next;
                node_to_delete->data->next_m = nullptr;
                delete node_to_delete->data; // Delete PlayerNode
                node_to_delete->next = nullptr;
                delete node_to_delete; // Delete node_to_player
            }
        }
        delete[] this->arr;
    }
    // TODO destructor
    StatusType check_array()
    {
        if ((double)num_elements_in_array / (double)array_size < 0.75)
            return StatusType::SUCCESS;
        // make new better bigger array
        
        array_size = array_size * EXPAND_FACTOR;
        node_to_player **newarr = new node_to_player *[array_size];
        if (newarr == nullptr)
            return StatusType::ALLOCATION_ERROR;
        
        for (int i = 0; i < array_size; i++)
        {
            newarr[i] = nullptr;
        }
        for (int i = 0; i < array_size / EXPAND_FACTOR; i++)
        {
            //std::cout <<arr[i]<< " in check array \n";
            node_to_player *temp = arr[i];
            // TODO check this while
            
            while (temp != nullptr)
            {
                //std::cout <<temp<< " in check array \n";
                int index = hash(temp->data->getPlayerId());
                node_to_player *new_arr_temp = newarr[index];
                newarr[index] = temp;
                temp = temp->next;
                newarr[index]->next = new_arr_temp;
            }
        }
        for (int i = 0; i < array_size / EXPAND_FACTOR; i++)
        {
            arr[i] = nullptr;
        }
        delete[] arr; // TODO might need to delete the node_to_player one by one
        this->arr = newarr;
        return StatusType::SUCCESS;
    }
    output_t<PlayerNode *> newPlayerNode(Player plr)
    {

        if (check_array() == StatusType::ALLOCATION_ERROR)
            return output_t<PlayerNode *>(StatusType::ALLOCATION_ERROR);
        // std::cout << plr.playerId_m<<" \n";
        PlayerNode *node_to_return = new PlayerNode(plr);
        node_to_player *newnode = new node_to_player;
        if (newnode == nullptr || node_to_return == nullptr)
        {
            return output_t<PlayerNode *>(StatusType::ALLOCATION_ERROR);
        }
        newnode->data = node_to_return;
        node_to_player *temp = this->arr[hash(plr.playerId_m)];
        this->arr[hash(plr.playerId_m)] = newnode;
        newnode->next = temp;
        this->num_elements_in_array++;
        return output_t<PlayerNode *>(node_to_return);
    }
    output_t<PlayerNode *> getPlayerNode(int id_player)
    {
        if(hash(id_player)>array_size-1||hash(id_player)<0)
            return output_t<PlayerNode *>(StatusType::FAILURE);
        node_to_player *temp = arr[hash(id_player)];
        while (temp != nullptr)
        {
            if (temp->data->getPlayerId() == id_player)
                return output_t<PlayerNode *>(temp->data);
            temp = temp->next;
        }
        return output_t<PlayerNode *>(StatusType::FAILURE);
    }
    int hash(int id)
    {
        return id % array_size;
    }
};

#endif // WET2_DYNAMIC_ARRAY