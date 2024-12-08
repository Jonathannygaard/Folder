Totaltime = GetTotalTime()
DeltaTime = GetDeltatime()

print(DeltaTime)


if Timer == nil then 
    Timer = 0
else
    Timer = Timer + DeltaTime
end
if Timer >= 5 then
    CreateSphere(10,10,100)
    Timer = 0
end

