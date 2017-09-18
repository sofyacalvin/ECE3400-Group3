# Milestone 1

For Milestone one, our team was required to make our robot follow a black line as well as complete a figure 8. 

## Following a Line

For the first part of the milestone, we had to figure out how to make our robot follow a line. We determined that the best way to approach this was to use sensors to read and track the line as the robot follows it. This means we were faced with two challenges: the placement and usage of sensors to detect the line, and the act of remaining on the line and following it as the robot moves.

### Hardware

### Software

To make our robot follow a line, we coded a way for the robot to utilize the two center sensors in front. First, we obtained data from the sensors to determine the values of white and black. From there, we created an algorithm represented by this pseudocode:

![Line Following Code](../images/linefollow.png "Line Following Code")

With this, if one sensor went off of the line, the robot would turn and correct itself to have both sensors over it again.

### Results

[![Line Following - Before](http://img.youtube.com/vi/RP49caLh4oo/0.jpg)](http://www.youtube.com/watch?v=RP49caLh4oo)

The video above shows our robot in action. It correctly follows a line, and corrects itself as it moves. However, we noticed that the robot had issues with remaining in a straight line, and felt that the turns were too jolting and unnecessarily slowed down the robot. It turned out that we initially failed to have a common ground for our servos, which resulted in them moving at different speeds. We also decided to have the robot use only one wheel at a time when adjusting so that the robot was always moving forward on a line, instead of pivoting to adjust. The final iteration can be seen below.

[![Line Following - After](http://img.youtube.com/vi/3n7LCLbF0UE/0.jpg)](http://www.youtube.com/watch?v=3n7LCLbF0UE)

With the first part done, we were ready to move on to the next step.

## Figure 8

For the figure 8, we faced several more challenges. The robot needed to follow a line, but it also needed to turn at and cross specific junctions. To do this, we needed the robot to determine where these junctions were, and how to act at these junctions.

### Hardware

### Software

The first task in coding our figure 8 program was implementing our line following algorithm. After that, we needed to tell the robot when it reached a junction, and what to do. Through the outer sensors, we were able to tell the robot when a "new action" was to be taken. The robot would then follow a loop of commands to determine whether that action was a turn or driving through an intersection.

For our turns, we first tried using an algorithm similar to this:

![Figure 8 - First Attempt](../images/figure81st.png "Figure 8 - First Attempt")

However, this proved to be unreliable. The robot would often begin turning but not complete the turn, or not turn at all. To fix this issue, we changed our code to implement a delay before the sensors would read new values. As a result, our robot was much more reliable.

![Figure 8 - Final](../images/figure8FINAL.png "Figure 8 - Final")

### Results

[![Figure 8 - Before](http://img.youtube.com/vi/FGEi17X3DoE/0.jpg)](http://www.youtube.com/watch?v=FGEi17X3DoE)

The video above shows our working, but unoptimized figure 8 build. We changed our turns to use both wheels (the outer wheel moves faster than the inner wheel to keep the turn sharp but not on a pivot) as well as moving our outer junction-detecting sensors back to compensate for quicker, sharper turns. Below is a video of our final implementation.

[![Figure 8 - After](http://img.youtube.com/vi/lCFXg24FKVM/0.jpg)](http://www.youtube.com/watch?v=lCFXg24FKVM)

