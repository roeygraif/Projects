#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef NODE_H_
#define NODE_H_

#include <stdbool.h>

typedef struct KeyValueNode_t *KeyValueNode;

typedef enum KeyValueNodeResult_t
{
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY,
    NODE_NULL_ARGUMENT,
    NODE_ITEM_ALREADY_EXISTS,
    NODE_ITEM_DOES_NOT_EXIST
} KeyValueNodeResult;

typedef void *NodeDataElement;

typedef void *NodeKeyElement;

typedef void (*freeNodeDataElements)(NodeDataElement);

typedef void (*freeNodeKeyElements)(NodeKeyElement);

KeyValueNode keyValueNodeCreate(NodeKeyElement key, NodeDataElement data, freeNodeKeyElements freeKey,freeNodeDataElements freeData);

void keyValueNodeDestroy(KeyValueNode node);

//KeyValueNode copyKeyValueNode(KeyValueNode src_node);

NodeKeyElement keyValueNodeGetKey(KeyValueNode node);

NodeDataElement keyValueNodeGetData(KeyValueNode node);

KeyValueNode getNext(KeyValueNode node);

void setKey(KeyValueNode node, NodeKeyElement newkey);

void setData(KeyValueNode node, NodeDataElement newdata);

void setNext(KeyValueNode node, KeyValueNode nextnew);

#endif /* MAP_H_ */
