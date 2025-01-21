#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>
#include "./node.h"

struct KeyValueNode_t
{
    freeNodeKeyElements freeKey;
    freeNodeDataElements freeData;

    NodeKeyElement key;
    NodeDataElement data;
    struct KeyValueNode_t *next;
};

KeyValueNode keyValueNodeCreate(NodeKeyElement key, NodeDataElement data, freeNodeKeyElements freeKey, freeNodeDataElements freeData)
{
    if(key == NULL || data == NULL){
        return NULL;
    }
    KeyValueNode new_node = malloc(sizeof(*new_node));
    if (new_node == NULL){
        return NULL;
    }
    new_node->key = key;
    new_node->data = data;
    new_node->next = NULL;
    new_node->freeKey = freeKey;
    new_node->freeData = freeData;
    
    return new_node;
}

void keyValueNodeDestroy(KeyValueNode node)
{
    if (node == NULL){
        return;
    }

    node->freeKey(node->key);
    node->freeData(node->data);
    free(node);
}

// KeyValueNode copyKeyValueNode(KeyValueNode src_node)
// {
//     if (src_node == NULL)
//         return NODE_ITEM_DOES_NOT_EXIST;
//     KeyValueNode newnode = malloc(sizeof(*newnode));
//     if (newnode == NULL)
//         return NODE_OUT_OF_MEMORY;
//     newnode->data = src_node->data;
//     newnode->key = src_node->key;
//     newnode->next = src_node->next;
//     return newnode;
// }

NodeKeyElement keyValueNodeGetKey(KeyValueNode node){
    if(node == NULL){
        return NULL;
    }
    return node->key;
}

NodeDataElement keyValueNodeGetData(KeyValueNode node){
    if(node == NULL){
        return NULL;
    }
    return node->data;
}

KeyValueNode getNext(KeyValueNode node){
    if(node == NULL){
        return NULL;
    }

    return node->next;
}

void setKey(KeyValueNode node, NodeKeyElement new_key){
    if(node == NULL || new_key == NULL){
        return;
    }
    node->key = new_key;
}

void setData(KeyValueNode node, NodeDataElement new_data){
    if(node == NULL || new_data == NULL){
        return;
    }
    node->data = new_data;
}

void setNext(KeyValueNode node, KeyValueNode new_next){
    if(node == NULL){
        return;
    }
    node->next = new_next;
}