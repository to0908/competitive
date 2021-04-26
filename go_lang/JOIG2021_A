package main

import (
	"fmt"
)

func main() {
	var a[3] int
	var ma, sum = 0, 0
	for i:=0;i<3;i++{
		fmt.Scan(&a[i])
		chmax(&ma,a[i])
		sum += a[i]
	}
	fmt.Println(ma*3-sum)

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
