
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fb.h"
#include "List.h"
#include "Map.h"
#include "Queue.h"

#define MAX_PEOPLE 128
#define MAX_RECOMMENDATIONS 20

struct fb {
    int   numPeople;

    char *names[MAX_PEOPLE]; // the id of a person is simply the index
                             // that contains their name in this array
    
    Map   nameToId; // maps names to ids
                    // question to think about: why do we have this when
                    // the names array already provides this information?

    bool  friends[MAX_PEOPLE][MAX_PEOPLE];
};

static char *myStrdup(char *s);
static int   nameToId(Fb fb, char *name);

////////////////////////////////////////////////////////////////////////

// Creates a new instance of FriendBook
Fb   FbNew(void) {
    Fb fb = malloc(sizeof(*fb));
    if (fb == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    fb->numPeople = 0;
    fb->nameToId = MapNew();

    for (int i = 0; i < MAX_PEOPLE; i++) {
        for (int j = 0; j < MAX_PEOPLE; j++) {
            fb->friends[i][j] = false;
        }
    }

    return fb;
}

void FbFree(Fb fb) {
    for (int i = 0; i < fb->numPeople; i++) {
        free(fb->names[i]);
    }

    MapFree(fb->nameToId);
    free(fb);
}

bool FbAddPerson(Fb fb, char *name) {
    if (fb->numPeople == MAX_PEOPLE) {
        fprintf(stderr, "error: could not add more people\n");
        exit(EXIT_FAILURE);
    }

    if (!MapContains(fb->nameToId, name)) {
        int id = fb->numPeople++;
        fb->names[id] = myStrdup(name);
        MapSet(fb->nameToId, name, id);
        return true;
    } else {
        return false;
    }
}

bool FbHasPerson(Fb fb, char *name) {
    return MapContains(fb->nameToId, name);
}

List FbGetPeople(Fb fb) {
    List l = ListNew();
    for (int id = 0; id < fb->numPeople; id++) {
        ListAppend(l, fb->names[id]);
    }
    return l;
}

bool FbFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    assert(id1 != id2);

    if (!fb->friends[id1][id2]) {
        fb->friends[id1][id2] = true;
        fb->friends[id2][id1] = true;
        return true;
    } else {
        return false;
    }
}

bool FbUnfriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    assert(id1 != id2);

    if (fb->friends[id1][id2]) {
        fb->friends[id1][id2] = false;
        fb->friends[id2][id1] = false;
        return true;
    } else {
        return false;
    }
}

bool FbIsFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    return fb->friends[id1][id2];
}

List FbGetFriends(Fb fb, char *name) {
    int id1 = nameToId(fb, name);
    
    List l = ListNew();
    for (int id2 = 0; id2 < fb->numPeople; id2++) {
        if (fb->friends[id1][id2]) {
            ListAppend(l, fb->names[id2]);
        }
    }
    return l;
}

int  FbNumFriends(Fb fb, char *name) {
    int id1 = nameToId(fb, name);
    
    int numFriends = 0;
    for (int id2 = 0; id2 < fb->numPeople; id2++) {
        if (fb->friends[id1][id2]) {
            numFriends++;
        }
    }
    return numFriends;
}

////////////////////////////////////////////////////////////////////////
// Your tasks

List FbMutualFriends(Fb fb, char *name1, char *name2) {
    List l = ListNew();

    // Retrieve name1 and name2's IDs
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    
    // Loop and compare a 3rd person's ID with name1 and name2's IDs
    for (int id3 = 0; id3 < fb->numPeople; id3++) {
        // If 3rd person is friends with both and their ID is unique, then mutual
        if (fb->friends[id1][id3] && fb->friends[id2][id3] && id1 != id3 && id2 != id3) {
            ListAppend(l, fb->names[id3]);
        }
    }

    return l;
}

void FbFriendRecs1(Fb fb, char *name) {
    int id1 =nameToId(fb, name);

    // Check numMutuals from highest to lowest so we can print in descending order
    // numMutals is at MOST (n - 2), i.e. everyone except the two being compared
    for (int numMutuals = fb->numPeople - 2; numMutuals > 0; numMutuals--) {
        // Check every person's mutuals with id1, if mutuals equal to current 'max' mutuals
        // and id1 is not friends with them, print as a recommendation
        for (int id2 = 0; id2 < fb->numPeople; id2++) {
            List mutuals = FbMutualFriends(fb, name, fb->names[id2]);
            if (ListSize(mutuals) == numMutuals && !fb->friends[id1][id2] && id1 != id2) {
                printf("\t%-20s%4d mutual friends\n", fb->names[id2], numMutuals);
            }
            // Free list since FbMutualFriends mallocs new list in function
            free(mutuals);
        }
    }
}

void FbFriendRecs2(Fb fb, char *name) {
    // Create a visited adjacency list for people in network
    bool visited[MAX_PEOPLE] = {false};

    // Traverse graph through BFS using Queue ADT
    int srcId = nameToId(fb, name);

    Queue q = QueueNew();
    QueueEnqueue(q, srcId);
    visited[srcId] = true;

    for (int i = 0; i < MAX_RECOMMENDATIONS && !QueueIsEmpty(q); i++) {
        printf("In this loop\n");
        int id1 = QueueDequeue(q);
        printf("After dequeue\n");

        // Don't print out the first item, since that's the src name
        if (id1 != srcId) printf("\t%s\n", fb->names[id1]);

        // If person hasn't been recommended yet, continue
        printf("BEFORE: i iteration on %d\n", i);
        if (visited[id1]) {
            printf("TRUE\n");
            continue;
        } else {
            printf("NOT TRUE\n");
        }
        printf("AFTER\n");


        // Find friends of friends of ... and enqueue
        for (int id2 = 0; id2 < fb->numPeople; id2++) {
            printf("In this for loop\n");
            if (fb->friends[id1][id2] && id1 != id2 && !visited[id2]) {
                printf("in this if statement\n");
                QueueEnqueue(q, id2);
                visited[id2] = true;
            }
        }
    }

    QueueFree(q);
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Converts a name to an ID. Raises an error if the name doesn't exist.
static int nameToId(Fb fb, char *name) {
    if (!MapContains(fb->nameToId, name)) {
        fprintf(stderr, "error: person '%s' does not exist!\n", name);
        exit(EXIT_FAILURE);
    }
    return MapGet(fb->nameToId, name);
}

