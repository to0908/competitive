package main

import (
	"fmt"
	"reflect"
	"sort"
	"bufio"
	"os"
)

func main() {
	// fmt.Fscan(read,&hoge,&fuga,...)
	// fmt.Fprintln(write,hoge,fuga,...)
	read := bufio.NewReader(os.Stdin)
    write := bufio.NewWriter(os.Stdout)
    defer write.Flush()

	var (
		n int
		m int
		d int
	)
	fmt.Fscan(read,&n,&m,&d)
	x := make([]Array,n)
	for i:=0;i<n;i++ {
		fmt.Fscan(read,&x[i].a, &x[i].b)
	}
	sort.Slice(x, func(i,j int) bool {
		return x[i].a < x[j].a
	})
	// fmt.Println(x)
	low:=-1
	up:=1000000007
	var mid int
	for up-low>1 {
		mid = (up+low)/2
		cnt := 0
		now := -d
		for i:=0;i<n;i++ {
			if x[i].a-now >= d && x[i].b >= mid {
				cnt++
				now = x[i].a
			}
		}
		if cnt >= m{
			low = mid
		}else{
			up = mid
		}
	}
	fmt.Fprintln(write,low)
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

// ソートされたkeyを返す。keyの型だけ指定する必要がある
func MapSortingString(mp interface{}) []string{
	keys := reflect.ValueOf(mp).MapKeys()
    res := make([]string, len(keys))
    for i, key := range keys {
        res[i] = key.String()
    }
    sort.Strings(res)
    return res
}
func MapSortingInt(mp interface{}) []int{
	keys := reflect.ValueOf(mp).MapKeys()
    res := make([]int, len(keys))
    for i, key := range keys {
        res[i] = int(key.Int())
    }
    sort.Ints(res)
    return res
}

type Array struct{
	a int
	b int
	// HOW TO SORT
	// x := make([]Array,n)
	// sort.Slice(x, func(i,j int) bool {
	// 	return x[i].a < x[j].a
	// })
}
