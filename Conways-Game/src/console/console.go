package console

import (
	"bufio"
	"bytes"
	"flag"
	"fmt"
	"game"
	"io"
	"io/ioutil"
	"os"
	"strconv"
	"strings"
	"time"
)

const (
	existsError  = "cannot access '%v': no such file or directory"
	fileError    = "cannot use '%v': not a regular file"
	parsingError = "invalid character '%c' in initial configuration: parse error"
)

var (
	wait         = flag.Duration("wait", time.Second, "the `duration` to wait after printing a generation")
	confirmGen   = flag.Bool("confirm", false, "whether to ask the user for confirmation after printing a generation")
	generations  = flag.Int("max-gens", -1, "the maximum number of `generations` to calculate")
	printNth     = flag.Uint("print-nth", 1, "print every nth `generation`, zero prints no generation")
	printInitial = flag.Bool("print-initial", false, "whether to print the initial state of the universe")
	printFinal   = flag.Bool("print-final", false, "whether to print the final state of the universe")
	printPadding = flag.Uint("print-padding", 1, "the amount of padding to use, when printing the universe")

	initialConfig = ""
	// add option for example initial states

)

func init() {
	flag.Usage = func() {
		printUsage()
		fmt.Fprintln(flag.CommandLine.Output())
		flag.PrintDefaults()
	}

}

func Main() {
	if !flag.Parsed() {
		parseArgs()
	}

	universe, err := parseConfiguration(initialConfig)
	if err != nil {
		fmt.Fprintln(flag.CommandLine.Output(), err)
		flag.Usage()
		os.Exit(2)
	}

	if *printInitial {
		fmt.Println("Initial configuration:")
		printCurrentState(universe)
		fmt.Println()
	}

	for gen, n := 0, 1; gen < *generations || *generations < 0; gen, n = gen+1, n+1 {
		universe.Evolve(8, 1)

		if *printNth != 0 && n%int(*printNth) == 0 {
			printCurrentState(universe)
			n = 0

			if !*confirmGen {
				time.Sleep(*wait)
			} else {
				fmt.Print("Press 'Enter' to continue...")
				bufio.NewReader(os.Stdin).ReadBytes('\n')
			}

			fmt.Println()
		}
	}

	if *printFinal {
		fmt.Println("Final configuration:")
		printCurrentState(universe)
	}
}

func printCurrentState(u game.Universe) {
	state, topLeft := u.CurrentState(int(*printPadding))
	botRight := []int{topLeft[0] + len(state[0]) - 1, topLeft[1] + len(state) - 1}
	stringWidth := 2*len(state[0]) + 1

	fmt.Printf("(%v,%v)\n", topLeft[0], topLeft[1])
	fmt.Println(toString(state))
	fmt.Printf("%"+strconv.Itoa(stringWidth)+"s\n", fmt.Sprintf("(%v,%v)", botRight[0], botRight[1]))

	fmt.Printf("Generation: %v\n", u.Generation())
	fmt.Println()
}

func toString(state [][]int) string {
	builder := strings.Builder{}

	for i, row := range state {
		if i > 0 {
			builder.WriteString("\n")
		}

		builder.WriteString(strings.ReplaceAll(strings.ReplaceAll(fmt.Sprint(row), "0", "-"), "1", "X"))
	}

	return builder.String()
}

func printUsage() {
	fmt.Fprint(flag.CommandLine.Output(), "usage: conways-game.go [flags] [file]\n",
		"use the contents of 'file' as initial state of the universe\n")
}

func parseArgs() {
	flag.Parse()

	if flag.NArg() != 1 {
		printUsage()
		os.Exit(2)
	}

	if info, err := os.Stat(flag.Arg(0)); err != nil {
		if os.IsNotExist(err) {
			fmt.Fprintf(flag.CommandLine.Output(), existsError, flag.Arg(0))
			fmt.Println()
			os.Exit(2)
		}
	} else if info.IsDir() {
		fmt.Fprintf(flag.CommandLine.Output(), fileError, flag.Arg(0))
		fmt.Println()
		os.Exit(2)
	}

	initialConfig = flag.Arg(0)
}

func parseConfiguration(path string) (game.Universe, error) {
	data, err := ioutil.ReadFile(path)
	if err != nil {
		return game.Universe{}, err
	}

	return parseUniverse(data)
}

func parseUniverse(data []byte) (game.Universe, error) {
	reader := bufio.NewReader(bytes.NewReader(data))
	state := make([][]int, 1, 4)

	for {
		if char, _, err := reader.ReadRune(); err != nil {
			if err == io.EOF {
				break
			} else {
				panic(err)
			}
		} else {
			switch char {
			case '\n':
				state = append(state, make([]int, 0, 4))
			case '-':
				state[len(state)-1] = append(state[len(state)-1], 0)
			case 'x':
				state[len(state)-1] = append(state[len(state)-1], 1)
			default:
				return game.Universe{}, fmt.Errorf(parsingError, char)
			}
		}
	}

	return game.NewUniverse(state), nil
}
