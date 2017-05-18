package mini

class Alignment(var coords: Array<IntArray>, var network: Array<IntArray>) {
  init {
    createNetwork()
  }
  fun displayCoords() {
    for (coord: IntArray in coords) {
      println("[${coord[0]}, ${coord[1]}]")
    }
  }

  fun displayNetwork() {
    for (row: IntArray in network) {
      for (col: Int in row) {
        print("$col ")
      }
      println()
    }
  }

  fun createNetwork() {
    for (coord: IntArray in coords) {
      network[coord[0]][coord[1]] = 1
    }
  }

  fun swap(node1: Int, node2: Int): Alignment {
    var newCoords = coords.copyOf()
    val temp = newCoords[node1]
    newCoords[node1] = newCoords[node2]
    newCoords[node2] = temp
    return Alignment(newCoords, Array(14, { IntArray(14) }))
  }

  fun move(node: Int, x: Int, y: Int): Alignment {
    var newCoords = coords.copyOf()
    newCoords[node] = intArrayOf(x, y)
    return Alignment(newCoords, Array(14, { IntArray(14) }))
  }

  fun neighbors(): Array<Alignment> {
    var neighbors = arrayOf<Alignment>()
    for (i in coords.indices) {
      for (j in coords.indices) {
        if (i != j) {
          neighbors.add(swap(i, j))
        }
      }
    }
    for (i in coords.indices) {
      for (j in network.indices) {
        for (k in network.indices) {
          if (!coords.contains(intArrayOf(j, k))) {
            neighbors.add(move(i, j, k))
          }
        }
      }
    }
    return neighbors
  }
}


fun main(args: Array<String>) {
  var network = Array(14, { IntArray(14) })
  var coords = Array(4, { IntArray(2) })
  var a = Alignment(coords, network)
  a.displayCoords()
  a.displayNetwork()
  a.swap(0, 1)
}
