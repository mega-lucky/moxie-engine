--!nocheck

const Transform = World.GetComponentID("Transform")
const MeshShape = World.GetComponentID("MeshShape")
const MeshRender = World.GetComponentID("MeshRender")

const entity = World.NewEntity()
World.GiveComponent(entity, Transform)
World.GiveComponent(entity, MeshShape)
World.GiveComponent(entity, MeshRender)

World.GetComponent(entity, MeshShape).Mesh = "Cube"
World.GetComponent(entity, MeshRender).Material = "Checkers"
World.GetComponent(entity, Transform).Position = vector.create(0, 0, -8)
World.GetComponent(entity, Transform).Scale = vector.create(1, 1, 1)
-- World.GetComponent(entity, Transform).Rotation = {0,0,0,1}

local query = World.Query(Transform)
local elapsed = 0

while true do
    const dt = time.wait()
    elapsed += dt

    query:Each(function(e)
        const t = World.GetComponent(entity, Transform)
        t.Position = vector.create(1.5 * math.sin(math.tau * elapsed), 0, -8)
    end)
end