void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

int reorient(current, next, int orientation){
  int currentInd = current[x]*5 + current[y] + orientation;
  int nextInd = next[x]*5 + next[y];

  val = abs(nextInd - currentInd);

  if(val == 3 || val == 6)
    left();
  else if(val == 5 || val == 4)
    right();
  else if(val == 0 || val == 1)
    forward();
  else
    flip();

  orientation = nextInd -(currentInd - orientation);

  if(orientation == -5)
    orientation += 1;
  else if (orientation == 5)
    orientation += -1;

  return orientation;
}

