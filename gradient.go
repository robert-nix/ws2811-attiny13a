package main

import "fmt"

type color struct {
	r, g, b uint8
}

func blend(c1, c2 color, out []color) {
	for i := range out {
		s := float64(i) / float64(len(out))
		out[i] = color{
			uint8(float64(c1.r) + (float64(c2.r)-float64(c1.r))*s),
			uint8(float64(c1.g) + (float64(c2.g)-float64(c1.g))*s),
			uint8(float64(c1.b) + (float64(c2.b)-float64(c1.b))*s),
		}
	}
}

func main() {
	// generates a 300-color rainbow for use by the mCU
	stops := []color{
		{255, 0, 0},   // red
		{255, 255, 0}, // yellow
		{0, 255, 0},   // green
		{0, 255, 255}, // cyan
		{0, 0, 255},   // blue
		{255, 0, 255}, // magenta
	}
	colors := make([]color, 100)
	for i := 0; i < len(stops); i++ {
		a := stops[i]
		b := stops[(i+1)%len(stops)]
		blend(a, b, colors[i*len(colors)/len(stops):(i+1)*len(colors)/len(stops)])
	}
	fmt.Printf("#define COLS_LEN %d\n", len(colors))
	// print red bytes for C
	fmt.Printf("const static unsigned char col_r[COLS_LEN] PROGMEM = {\n  ")
	for i, c := range colors {
		fmt.Printf("0x%02x, ", c.r)
		if (i % 10) == 9 {
			if (i + 1) != len(colors) {
				fmt.Printf("\n  ")
			} else {
				fmt.Printf("\n};\n")
			}
		}
	}
	// print green bytes for C
	fmt.Printf("const static unsigned char col_g[COLS_LEN] PROGMEM = {\n  ")
	for i, c := range colors {
		fmt.Printf("0x%02x, ", c.g)
		if (i % 10) == 9 {
			if (i + 1) != len(colors) {
				fmt.Printf("\n  ")
			} else {
				fmt.Printf("\n};\n")
			}
		}
	}
	// print blue bytes for C
	fmt.Printf("const static unsigned char col_b[COLS_LEN] PROGMEM = {\n  ")
	for i, c := range colors {
		fmt.Printf("0x%02x, ", c.b)
		if (i % 10) == 9 {
			if (i + 1) != len(colors) {
				fmt.Printf("\n  ")
			} else {
				fmt.Printf("\n};\n")
			}
		}
	}
}
