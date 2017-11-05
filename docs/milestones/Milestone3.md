# Milestone 3

## Objective
Our objective for Milestone 3 was to implement a search algorithm in simulation as well as in real life. In both, the robot must display a "done" signal at the end of the search (i.e. once all explorable squares have been visited).

## Procedure

### Simulation
We chose to write our simulation in Python, due to the simplicity of interfacing with graphics. We used the [Pygame library](https://www.pygame.org/) for the display, which is often used for simple game development. To create the virtual environment that would simulate the real-life maze, we wrote two classes. The class Square would create each "square" (i.e. intersection in the maze) with attributes describing its (x,y) coordinate, index (from 0-19), and if any walls surround the square.

```
class Square():
	""" 
	Instance is each grid square.
	"""

	def __init__(self, x, y, index, right, down, left, up):
		self.x = x
		self.y = y
		self.index = index
		self.right = right
		self.down = down
		self.left = left
		self.up = up
```

The Maze class initializes the maze itself, which is a 2D array of Square objects. There are functions within this class that assist with its usability. makeSquares() creates the 2D array, assigning the coordinates and indicies; and setupWalls() uses two presumably assigned "horizontal wall" and "vertical wall" 2D arrays to assign each Square's directional wall attributes.

```
	def makeSquares(self):
		x = 0
		y = 0
		index = 0
		right = 0
		down = 0
		left = 0
		up = 0
		for row in range(ROWS):
			for col in range(COLS):
				self.squares[row][col] = (Square(row, col, index, right, down, left, up))
				index = index + 1

	def setupWalls(self, hwall, vwall):
		for row in range(ROWS):
			for col in range(COLS):
				if hwall[row][col] == 1:
					self.squares[row][col].up = 1

				if hwall[row + 1][col] == 1:
					self.squares[row][col].down = 1

				if vwall[row][col] == 1:
					self.squares[row][col].left = 1

				if vwall[row][col + 1] == 1:
					self.squares[row][col].right = 1
```

After initializing the squares, Maze, walls, and the Pygame library, we define two more functions--depth first search, and drawing the maze. Since Python allows modification of the size of lists, we initialize three lists to use as stacks--visited (i.e. which squares the robot has gone to), a frontier (in order, the next squares to visit), and a path (in order, the squares visited) to facilitate backtracking when it is needed. We use a variable "goback" to determine if a dead-end is hit, whether with walls or due to all adjacent squares being visited.


### Robot

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
