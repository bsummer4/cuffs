-- # Physics Simulation
--
-- We accept the following messages from stdin:
-- - move playerName { left | right | up | down }
-- - shoot playerName angle power
-- - place playerName position     <-- This may only happen once
-- - advance                       <-- Move forward a timestep

import Data.Set (Set)
import qualified Data.Set as Set
import List


-- ## Misc
swap (x, y) = (y, x)
toSet' s [] = s
toSet' s (x:xs) = toSet' (Set.insert x s) xs
toSet xs = toSet' Set.empty xs
first (x:xs) = x
firstL (x:xs) = [x]
firstL [] = []
cmp f x1 x2 = if (v1 < v2) then LT else GT
              where v1 = f x1
                    v2 = f x2


-- ## Explosion
expln x y r = (x,y,r)


-- ## gmap
gmap w h solid = (w,h,solid)
expld (x,y,r) (w,h,s) = (w,h,(Set.intersection s $ toSet $ circ x y r)) -- TODO


-- ## Projectiles (TODO Wrapping)
proj x y dx dy = ((x, y), (dx, dy))
move ((x,y), (dx,dy)) = ((x+dx,y+dy), (dx,dy))
moves p 0 = p
moves p n = moves (move p) $ n-1
accel ((x,y), (dx,dy)) (ddx, ddy) = ((x,y), (dx+ddx,dy+ddy))


-- ## Energy Tracking
energyRechargeRate = 0.05
energyMax = 15.0
rockCost = 1.0
recharge = min energyMax . (+ energyRechargeRate)
enoughEnergy e cost = (e >= cost)
consume e cost = max 0 (e - cost)


-- ## Map Solidity
distance (x,y) (x',y') = sqrt ((x-x')^2 + (y-y')^2)
slope (x,y) (x',y') = if xd == 0 then 0 else  yd/xd where xd=(x'-x); yd=(y'-y)
pointsAround p radius = []
findHits p p' isSolid = filter isSolid $ line p p'
findHits' p p' solids = findHits p p' (`Set.member` solids)
closestHits p p' solids = sortBy (cmp $ distance p) (findHits' p p' solids)
closestHit p p' solids = firstL $ closestHits p p' solids
circ x y r = [(x,y)] -- TODO

-- This implements Bresenham's line algorithm. We return the points in
-- whatever order we find most convient.
line (x, y) (x', y') = if abs(y' - y) > abs(x' - x)
    then map swap $ line (y, x) (y', x')
    else if (x < x')
         then line' x' y' (slope (x,y) (x',y')) ystep x y 0.0
         else line (x', y') (x, y)
    where ystep = if (y < y') then 1 else -1

line' maxX maxY de ystep x y e = if x > maxX
      then []
      else (x, y):(recur y' e'')
          where recur = (line' maxX maxY de ystep (1+x))
                e' = e + de
                e'' = if e' > 0.5 then e' - 1 else e'
                y' = if e' > 0.5 then y + ystep else y
