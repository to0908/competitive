package main

import (
	"bufio"
	"fmt"
	"os"
	"reflect"
	"sort"
	"container/heap"
	"strconv"
)



func main() {
	// fmt.Fscan(read,&hoge,&fuga,...)
	// fmt.Fprintln(write,hoge,fuga,...)
	write := bufio.NewWriter(os.Stdout)
	defer write.Flush()
	// read := bufio.NewReader(os.Stdin)　// <-どちらか一方のみ 
	buf := make([]byte, 1024*1024)
	sc.Buffer(buf, bufio.MaxScanTokenSize)
	sc.Split(bufio.ScanWords)

	n:=readInt()
	m:=readInt()
	k:=readInt()
	q:=readInt()
	a1 := make([]int,0)
	a2 := make([]int,0)
	for i:=0;i<n;i++{
		p:=readInt()
		t:=readInt()
		if t!=0 {
			a2 = append(a2, p)
		}else{
			a1 = append(a1, p)
		}
	}
	if len(a1)!=0 { sort.Ints(a1) }
	if len(a2)!=0 { sort.Ints(a2) }
	ans := int(1e18)
	sum := 0
	cnt := 0
	for i:=0;i<len(a1);i++{
		cnt++
		sum+=a1[i]
		if cnt == m {break}
	}
	r:=cnt-1
	now := 0
	for i:=0;cnt<m;i++{
		cnt++
		sum+=a2[i]
		now = i+1
		if i % k == 0{
			sum += q
		}
	}
	ans = sum
	for i:=now;i<len(a2) && r>=0;i++{
		sum += a2[i]
		sum -= a1[r]
		r--
		if i%k==0{
			sum += q
		}
		chmin(&ans,sum)
	}
	fmt.Fprintln(write,ans)

	
}


// io -------------------------
var sc = bufio.NewScanner(os.Stdin)
func readInt() int {
	sc.Scan()
	i, _ := strconv.Atoi(sc.Text())
	return i
}
// --------------------------------

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
	a,b int
	// HOW TO SORT
	// x := make([]Array,n)
	// sort.Slice(x, func(i,j int) bool {
	// 	return x[i].a < x[j].a
	// })
}

type Edge struct {
	to, cost int
}
type WeightedGraph struct {
	n int
	v [][]Edge
}
func NewWeightedGraph(n int) WeightedGraph {
	g := WeightedGraph{n,make([][]Edge,n)}
	return g
}
func (g *WeightedGraph) add_directed_edge(from, to, cost int){
	g.v[from] = append(g.v[from],Edge{to,cost})
}
func (g *WeightedGraph) add_undirected_edge(u, v, cost int){
	g.v[u] = append(g.v[u],Edge{v,cost})
	g.v[v] = append(g.v[v],Edge{u,cost})
}
type State struct { dist, p int }
type Dijkstra []State
func (pq Dijkstra) Len() int { return len(pq) }
func (pq Dijkstra) Less(i, j int) bool { return pq[i].dist < pq[j].dist }
func (pq Dijkstra) Swap(i, j int) { pq[i], pq[j] = pq[j], pq[i] }
func (pq *Dijkstra) Push(x interface{}) { *pq = append(*pq, x.(State)) }
func (pq *Dijkstra) Pop() interface{} {
	x := (*pq)[len(*pq)-1]
	*pq = (*pq)[0 : len(*pq)-1]
	return x
}
func (g *WeightedGraph) shortest_path(s int)[]int{
	n := g.n
	INF := 100000000000000000+7
	dp := make([]int,n)
	for i:=0;i<n;i++ {
		dp[i] = INF
	}
	pq := new(Dijkstra)
	heap.Push(pq,State{0,s})
	for pq.Len() != 0 {
		cur := heap.Pop(pq).(State)
		if(dp[cur.p] < cur.dist){ continue }
		for i:=0;i<len(g.v[cur.p]);i++{
			nv := g.v[cur.p][i].to
			ndist := cur.dist + g.v[cur.p][i].cost
			if dp[nv] > ndist{
				dp[nv] = ndist
				heap.Push(pq,State{ndist,nv})
			}
		}
	}
	return dp
}
