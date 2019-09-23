package game

// Universe contains the current generation of a universe, as well as other
// important information about it.
type Universe struct {
	current, alternate [][]int
	topLeft            [2]int
	generation         int
}

// NewUniverse creates a new universe and populates it, using the specified slice.
func NewUniverse(initial [][]int) Universe {
	if isEmpty(initial) {
		return emptyUniverse(0)
	}

	lenX, lenY := len(initial[0]), len(initial)
	capX, capY := 2*lenX, 2*lenY

	current := make([][]int, lenY, capY)
	alternate := make([][]int, lenY, capY)

	for y := range current {
		current[y] = make([]int, lenX, capX)
		alternate[y] = make([]int, lenX, capX)
	}

	for y := range initial {
		copy(current[y], initial[y])
	}

	universe := Universe{current, alternate, [2]int{0, 0}, 0}
	universe.ensurePadding(4, 4, 4)

	return universe
}

// Evolve calculates the next n generations of the universe.
func (u *Universe) Evolve(padding, generations int) {
	for n := 0; n < generations; n++ {
		u.evolveUniverse()
		u.ensurePadding(1, 2*padding, padding)
	}
}

// CurrentState returns a copy of the current generation with p rows/columns of
// padding. The second return value is the position upper left corner of the copy.
func (u Universe) CurrentState(p int) ([][]int, [2]int) {
	state := make([][]int, len(u.current))

	for y := range u.current {
		state[y] = make([]int, len(u.current[y]))
		copy(state[y], u.current[y])
	}

	old, new := padding(state), [4]int{}
	state, new = ensurePadding(state, old, p, p, p)

	return state, calculateTopLeft(u.topLeft, old, new)
}

// Generation returns the number of the current generation of a Universe
func (u Universe) Generation() int {
	return u.generation
}

func emptyUniverse(generation int) Universe {
	return Universe{[][]int{[]int{0}}, [][]int{[]int{0}}, [2]int{0, 0}, generation}
}

func (u *Universe) evolveUniverse() {
	for y := range u.current {
		for x, cell := range u.current[y] {
			if cell != 0 {
				u.increaseNeighbourhood(x, y)
			}
		}
	}

	for y := range u.alternate {
		for x, cell := range u.alternate[y] {
			if cell == 3 {
				u.alternate[y][x] = 1
			} else if cell == 2 && u.current[y][x] == 1 {
				u.alternate[y][x] = 1
			} else {
				u.alternate[y][x] = 0
			}

			u.current[y][x] = 0
		}
	}

	u.current, u.alternate = u.alternate, u.current
	u.generation++
}

func (u Universe) increaseNeighbourhood(x, y int) {
	for dy := -1; dy <= 1; dy++ {
		for dx := -1; dx <= 1; dx++ {
			if dx == 0 && dy == 0 {
				continue
			}

			if x+dx >= 0 && y+dy >= 0 && x+dx < len(u.current[0]) && y+dy < len(u.current) {
				u.alternate[y+dy][x+dx]++
			}
		}
	}
}

func calculateTopLeft(topLeft [2]int, oldPadding, newPadding [4]int) [2]int {
	return [2]int{topLeft[0] + (oldPadding[2] - newPadding[2]),
		topLeft[1] + (oldPadding[0] - newPadding[0])}
}

func isEmpty(s [][]int) bool {
	for y := range s {
		for _, c := range s[y] {
			if c > 0 {
				return false
			}
		}
	}

	return true
}
