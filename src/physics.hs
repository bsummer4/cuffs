import Data.Set (Set)
import qualified Data.Set as Set

-- We accept the following messages from stdin:
-- - move playerName { left | right | up | down }
-- - shoot playerName angle power
-- - place playerName position     <-- This may only happen once
-- - advance                       <-- Move forward a timestep


-- ## Energy Tracking
energyRechargeRate = 0.05
energyMax = 15.0
rockCost = 1.0
recharge = min energyMax . (+ energyRechargeRate)
enoughEnergy e cost = (e >= cost)
consume e cost = max 0 (e - cost)

-- ## Map Solidity
distance (x,y) (x',y') = sqrt ((x-x')^2 + (y-y')^2)
slope (x,y) (x',y') = if xd == 0 then 0 else yd/xd
                    where yd = y - y'
                          xd = x - x'

pointsAround p0 radius = []
findHit p0 p1 isSolid = let hits = filter isSolid $ line p0 p1
                            in hits

-- ... solid =  p0 p1 (`set.member` solid)

-- Assume x < x' and 0 < slope < 1.
line (x,y) (x',y') = let dx = x' - x
                         dy = y' - y
                         de = dy / dx
                         in line' x' y' de x y 0.0

line' maxX maxY de x y e = if x > maxX
      then []
      else (x,y):(recur y' e'')
          where recur = (line' maxX maxY de (1+x))
                e' = e + de
                e'' = if (abs e') > 0.5
                      then e' - 1
                      else e'
                y' = if (abs e') > 0.5
                     then y + 1
                     else y
