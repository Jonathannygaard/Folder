Totaltime = GetTotalTime()
DeltaTime = GetDeltatime()

if Timer == nil then 
    Timer = 0
else
    Timer = Timer + DeltaTime
end
if Timer >= 1 then
    CreateSphere(100,10,100)
    Timer = 0
end

if TrackTimer == nil then
    TrackTimer = 0
else
    TrackTimer = TrackTimer + DeltaTime
end
if TrackTimer >= 0.1 then
    --TrackSpheres()
    TrackTimer = 0
end
