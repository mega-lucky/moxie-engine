--!nocheck

const Transform = World.GetComponentID("Transform")
const MeshShape = World.GetComponentID("MeshShape")
const MeshRender = World.GetComponentID("MeshRender")

time.wait(3)

const entity = World.NewEntity();
World.GiveComponent(entity, Transform)
World.GiveComponent(entity, MeshShape)
World.GiveComponent(entity, MeshRender)

World.GetComponent(entity, MeshShape).Mesh = "Cube"
World.GetComponent(entity, MeshRender).Material = "Checkers"
const t = World.GetComponent(entity, Transform)
t.Position = vector.create(0, 0, -12)
t.Scale = vector.create(1, 1, 1)
t.Rotation = {0,0,0,1}

while t.Position.z < -3 do
    t.Position += vector.create(0, 0, time.wait())
end
time.wait(5)
local elapsed = 0
while true do
    elapsed += time.wait()
    t.Rotation = {0, math.sin(elapsed/2), 0, math.cos(elapsed/2)}
end
World.DeleteEntity(entity)