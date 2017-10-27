"""
 Example program to show using an array to back a grid on-screen.
 
 Sample Python/Pygame Programs
 Simpson College Computer Science
 http://programarcadegames.com/
 http://simpson.edu/computer-science/
 
 Explanation video: http://youtu.be/mdTeqiWyFnc
"""

import pygame
 
# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
ORANGE = (255, 128, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0 , 255)
GREY = (192, 192, 192)

 
# This sets the WIDTH and HEIGHT of each grid location
WIDTH = 100
HEIGHT = 100
 
# This sets the margin between each cell
MARGIN = 10
 
# Create a 2 dimensional array. A two dimensional
# array is simply a list of lists.
grid = []
for row in range(4): # number rows
    grid.append([])
    for column in range(5): # number columns
        grid[row].append(0) 

wall = []
for row in range(3): # rows
    wall.append([])
    for column in range(4): # columns
        wall[row].append(0) 
 
# Initialize pygame
pygame.init()
 
# Set the HEIGHT and WIDTH of the screen
WINDOW_SIZE = [560, 450]
screen = pygame.display.set_mode(WINDOW_SIZE)
 
# Set title of screen
pygame.display.set_caption("Maze Simulation")
 
# Loop until the user clicks the close button.
done = False
 
# Used to manage how fast the screen updates
clock = pygame.time.Clock()

start = [0,0]
grid[start[0]][start[1]] = 1

def dfs(graph, start):
    visited, frontier = [], [start]
    while frontier:
        current = frontier.pop()

        print "current is " 
        print current
        print type(current)

        if current not in visited:
            visited.append(current)
            grid[current[0]][current[1]] = 1
            
            """ DEFINE NEXT """

            """
            if current[1] = 4: # end of row


        	else:
        		next = [current[0], current[1] + 1] # right one
        	"""

            
            try: 
            	next = [current[0], current[1] + 1] # right one
            	grid[next[0]][next[1]]
            except IndexError:
            	try:
            		next = [current[0] + 1, current[1]] #down one
            		grid[next[0]][next[1]]
            	except IndexError:
            		try:
            			next = [current[0], current[1] - 1] #left one
            			grid[next[0]][next[1]]
            		except IndexError:
            			next = [current[0] - 1, current[1]] #up one
            			grid[next[0]][next[1]]
			

            print "next is " + str(next)
            frontier.append(next)
    return visited

# -------- Main Program Loop -----------
while not done:
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop
        elif event.type == pygame.MOUSEBUTTONDOWN:
            # User clicks the mouse. Get the position
            pos = pygame.mouse.get_pos()
            # Change the x/y screen coordinates to grid coordinates
            column = pos[0] // (WIDTH + MARGIN)
            row = pos[1] // (HEIGHT + MARGIN)
            # Set that location to one
            # grid[row][column] = 1
            print("Grid coordinates: ", row, column)
	
	dfs(grid, start)
	# grid[1][1] = 1
 
    # Set the screen background
    screen.fill(GREY)
 
    # Draw the grid
    for row in range(4):
        for column in range(5):
            color = WHITE
            if grid[row][column] == 1:
                color = GREEN
            pygame.draw.rect(screen,
                             color,
                             [(MARGIN + WIDTH) * column + MARGIN,
                              (MARGIN + HEIGHT) * row + MARGIN,
                              WIDTH,
                              HEIGHT])
 
    # Limit to 60 frames per second
    clock.tick(60)
 
    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()
 
# Be IDLE friendly. If you forget this line, the program will 'hang'
# on exit.
pygame.quit()