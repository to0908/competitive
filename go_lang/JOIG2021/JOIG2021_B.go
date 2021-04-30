package main

import (
	"fmt"
	"strings"
)

func solve(s string) string {
	var ret strings.Builder
	ret.Grow(len(s))
	for i:=0;i<len(s);i++{
		c := s[i]
		if c>='a'{
			c -= 'a' - 'A';
		}else{
			c += 'a' - 'A'
		}
		ret.WriteByte(c)
	}
	return ret.String()
}

func main() {
	var (
		n int
		k int
		s string
	)
	fmt.Scan(&n,&k,&s)
	fmt.Println(s[:k-1]+solve(s[k-1:]))
}

// chmax(&a,b)として使う
func chmax(a *int, b int) bool {
	if *a>=b{
		return false
	}else{
		*a=b
		return true
	}
}

func chmin(a *int, b int) bool {
	if *a<=b{
		return false
	}else{
		*a=b
		return true
	}
}
