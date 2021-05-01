package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	// fmt.Fscan(read,&hoge,&fuga,...)
	// fmt.Fprintln(write,hoge,fuga,...)
	read := bufio.NewReader(os.Stdin)
    write := bufio.NewWriter(os.Stdout)
    defer write.Flush()

	var s string
	fmt.Fscan(read,&s)
	ok := true
	if s[3] != '-'{
		ok = false
	}
	for i:=0;i<8;i++{
		if i == 3{
			continue
		}
		if s[i]>='0' && s[i]<='9'{
			continue
		}
		ok = false
	}
	if ok {
		fmt.Println("Yes\n")
	}else{
		fmt.Println("No\n")
	}
	
}
