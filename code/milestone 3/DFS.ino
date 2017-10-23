#include <StackList.h>

StackList<unsigned char> frontier;
unsigned char visited[20];  

unsigned x_pos = 0;
unsigned y_pos = 0;
int index = 0;
int orientation = 0;

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:

  frontier.push(x_pos << 2 | y_pos);
  
  while(!frontier.isEmpty()) {
    unsigned char current = frontier.pop();
    //move to current
    index = (y_pos * 5) + x_pos;
    
    if (!visited[index]) { //current is unvisited
      addFrontier();
      visited[index] = 1;
    }
  }


  
  
}
// starting from top left (refer to map)
void addFrontier(){
  unsigned char next;
  if (orientation == 0) { //facing east
    if (1) { //north is path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
    }
    if (1){//south is path
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
    }
    if (1){//east is path--most prioritized
      //assuming receieved data ignores "state data"
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
    }
  }
  
  else if (orientation == 1) { // facing south
    if(1) { // west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
    }
    if (1) { //south path
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
    }
    if (1) { //east
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
    }
  }
  
  else if (orientation == 2) { // facing west
    if(1) { // north path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
    }
    if (1) { //west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
    }
    if (1) { //south
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
    }
  }
  else if (orientation == 3) { // facing north
    if(1) { // west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
    }
    if (1) { //north path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
    }
    if (1) { //east
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
    }
  }
}

