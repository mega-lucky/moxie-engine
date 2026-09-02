--!nocheck
--!optimize 2

const Transform = World.GetComponentID("Transform")
const MeshShape = World.GetComponentID("MeshShape")
const MeshRender = World.GetComponentID("MeshRender")

const entity = World.NewEntity()
World.GiveComponent(entity, Transform)
World.GiveComponent(entity, MeshShape)
World.GiveComponent(entity, MeshRender)

World.GetComponent(entity, MeshShape).Mesh = "Cube"
World.GetComponent(entity, MeshRender).Material = "Checkers"

const transform = World.GetComponent(entity, Transform);
transform.Position = vector.create(0, 0, -8)
transform.Scale = vector.create(1, 1, 1)

while true do
    const dt = time.wait()
    if Input.IsKeyDown(Input.KeyInput.D) then
        transform.Position += vector.create(5 * dt, 0, 0)
    end
    if Input.IsKeyDown(Input.KeyInput.A) then
        transform.Position += vector.create(-5 * dt, 0, 0)
    end
    if Input.IsKeyDown(Input.KeyInput.W) then
        transform.Position += vector.create(0, 5 * dt, 0)
    end
    if Input.IsKeyDown(Input.KeyInput.S) then
        transform.Position += vector.create(0, -5 * dt, 0)
    end
    if Input.IsMouseButtonDown(Input.MouseInput.Left) then
        transform.Scale += vector.one * dt * 5;
    end
    if Input.IsMouseButtonDown(Input.MouseInput.Right) then
        transform.Scale -= vector.one * dt * 5;
    end
end