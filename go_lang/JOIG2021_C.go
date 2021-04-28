package main

import (
	"fmt"
	"reflect"
	"sort"
)

func main() {
	var n int
	fmt.Scan(&n)
	var a = make([]int,n)
	for i:=0;i<n;i++{
		fmt.Scan(&a[i])
	}
	sum := 0
	for i:=0;i<n;i++{
		if a[i] == 1{
			sum++
		}
	}
	ans := sum
	for i:=0;i<n;i++ {
		if a[i] == 1{
			sum--;
		}else{
			sum++;
		}
		chmin(&ans,sum)
	}
	fmt.Println(ans)
	
	
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

// ソートされたkeyを返す。型だけ指定する必要がある
func MapSortingS(mp interface{}) []string{
	values := reflect.ValueOf(mp).MapKeys()
    result := make([]string, len(values))
    for i, value := range values {
        result[i] = value.String()
    }
    sort.Strings(result)
    return result
}
func MapSortingI(mp interface{}) []int{
	values := reflect.ValueOf(mp).MapKeys()
    result := make([]int, len(values))
    for i, value := range values {
        result[i] = int(value.Int())
    }
    sort.Ints(result)
    return result
}
