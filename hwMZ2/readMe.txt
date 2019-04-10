dijkstras algorithm------------------------------------------------
sets all of the weights to infinity
sets starting weight to 0
adds vertex to priority Q
if the Q is not empty 
get the smallest in the Q
for each of its neighbours
relax(decrease key)
repeat


Q.front should return the pair<vertex*, priority>
i can use the priority to calculate the new priority for the relax function

i need to find the priority for any vertex
the index it finds is incorrect





create graph--------------------------------------------------------
vector of neighbours(edges)
 contains a vertex and the weight to it
if i am at a number and theres an adjacent vertex create edge with weight 1 to it
when i find the matching number add an adge to it with weight equal to its number

if i am at a space or a number create a vertex
add to list of vertices
if there is a space or number next to it
  if that number is not in the list of portals
  add edge to it with weight 1
  if the number is in the

there should be a list of portal weight and vertices* 
if i am at a number i should check if the number is in the portal list
if it is add an edge to it with weight of that number
