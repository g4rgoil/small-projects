package game

type side int

const (
	top    side = iota
	bottom side = iota
	left   side = iota
	right  side = iota
)

var sides = [4]side{top, bottom, left, right}

func (u *Universe) ensurePadding(min, max, reset int) {
	old, new := u.padding(), [4]int{}

	u.current, new = ensurePadding(u.current, old, min, max, reset)
	u.alternate, _ = ensurePadding(u.alternate, old, min, max, reset)

	u.topLeft = calculateTopLeft(u.topLeft, old, new)
}

func ensurePadding(s [][]int, padding [4]int, min, max, reset int) ([][]int, [4]int) {
	for i, side := range sides {
		if padding[i] >= 0 {
			if padding[i] < min {
				s = addPadding(s, reset-padding[i], side)
				padding[i] = reset
			} else if padding[i] > max {
				s = removePadding(s, padding[i]-reset, side)
				padding[i] = reset
			}
		}
	}

	return s, padding
}

func (u Universe) padding() [4]int {
	return padding(u.current)
}

func (u Universe) paddingTop() int {
	return paddingTop(u.current)
}

func (u Universe) paddingBottom() int {
	return paddingBottom(u.current)
}

func (u Universe) paddingLeft() int {
	return paddingLeft(u.current)
}

func (u Universe) paddingRight() int {
	return paddingRight(u.current)
}

func padding(s [][]int) [4]int {
	return [4]int{paddingTop(s), paddingBottom(s), paddingLeft(s), paddingRight(s)}
}

func paddingTop(s [][]int) int {
	for y := range s {
		for _, cell := range s[y] {
			if cell != 0 {
				return y
			}
		}
	}

	return -1
}

func paddingBottom(s [][]int) int {
	for y := range s {
		for _, cell := range s[len(s)-y-1] {
			if cell != 0 {
				return y
			}
		}
	}

	return -1
}

func paddingLeft(s [][]int) int {
	for x := range s[0] {
		for y := range s {
			if s[y][x] != 0 {
				return x
			}
		}
	}

	return -1
}

func paddingRight(s [][]int) int {
	for x := range s[0] {
		for y := range s {
			if s[y][len(s[y])-x-1] != 0 {
				return x
			}
		}
	}

	return -1
}

func addPadding(s [][]int, n int, side side) [][]int {
	switch side {
	case top:
		return addPaddingTop(s, n)
	case bottom:
		return addPaddingBottom(s, n)
	case left:
		return addPaddingLeft(s, n)
	case right:
		return addPaddingRight(s, n)
	}

	return s
}

func addPaddingTop(s [][]int, n int) [][]int {
	s = append(s, make([][]int, n, n)...)

	copy(s[n:], s[0:])

	for i := 0; i < n; i++ {
		s[i] = make([]int, len(s[n]), cap(s[n]))
	}

	return s
}

func addPaddingBottom(s [][]int, n int) [][]int {
	for i := 0; i < n; i++ {
		s = append(s, make([]int, len(s[0]), cap(s[0])))
	}

	return s
}

func addPaddingLeft(s [][]int, n int) [][]int {
	for y := range s {
		s[y] = append(s[y], make([]int, n, n)...)

		copy(s[y][n:], s[y][0:])

		for i := 0; i < n; i++ {
			s[y][i] = 0
		}
	}

	return s
}

func addPaddingRight(s [][]int, n int) [][]int {
	for y := range s {
		s[y] = append(s[y], make([]int, n, n)...)
	}

	return s
}

func removePadding(s [][]int, n int, side side) [][]int {
	switch side {
	case top:
		return removePaddingTop(s, n)
	case bottom:
		return removePaddingBottom(s, n)
	case left:
		return removePaddingLeft(s, n)
	case right:
		return removePaddingRight(s, n)
	}

	return s
}

func removePaddingTop(s [][]int, n int) [][]int {
	return s[n:]
}

func removePaddingBottom(s [][]int, n int) [][]int {
	return s[:len(s)-n]
}

func removePaddingLeft(s [][]int, n int) [][]int {
	for y := range s {
		s[y] = s[y][n:]
	}

	return s
}

func removePaddingRight(s [][]int, n int) [][]int {
	for y := range s {
		s[y] = s[y][:len(s[y])-n]
	}

	return s
}
