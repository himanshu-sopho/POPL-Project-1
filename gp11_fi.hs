import Data.Array.IO
import Control.Monad
import System.IO 
import Data.List (insert)
import Data.Bool
import Data.IORef
{-#LANGUAGE ScopedTypeVariables#-}
f :: [String] -> [Int]
f = map read
getNumber::IO Int
getNumber = do
    num <- getLine
    return (read num)
inc:: Int -> Int 
inc n = n+1   
main = do
	let list = []
      --  print list 
        
        arr1 <- newArray (0,9) 0 :: IO (IOArray Int Int)
        arr2 <- newArray (0,9) 0 :: IO (IOArray Int Int)
        arr3 <- newArray (0,9) 0 :: IO (IOArray Int Int)
        arr4 <- newArray (0,9) 0 :: IO (IOArray Int Int)
        handle <- openFile "todo.txt" ReadMode
       
       -- let lists= tail list
        
       --let list = []
      -- handle <- openFile "todo.txt" ReadMode
        contents <- hGetContents handle
        let singlewords = words contents
            list = f singlewords	
       --   arr = f singlewords
       --print arr
       --hClose handle    
        --print (a,b,c)
       -- print arr1
       -- print arr2
        let lists=(list++[-1])
        i<-newIORef(0::Int)
     --  print n
        let l= False
        let loop= do
       	 	 --  putStrLn "Please input a number."
       	 	 --  contents <- hGetLine handle
        	  -- let singlewords = words contents
            	    --   list = f singlewords
        	  -- print list
        	   out<-readIORef i
        	   let n= (lists!!out)
       	 	  -- let lists= tail list
        	   --print lists 
       	 	   --print n
       	 	   let j= mod (n*5) 10
       	 	   a <- readArray arr1 j
       	 	   let k=a+1
	 	   writeArray arr1 j k
	 	   let j= mod (n*7) 10
	 	   a <- readArray arr2 j
	 	   let k=a+1
	 	   writeArray arr2 j k
	 	   let j= mod (n*13) 10
	 	   a <- readArray arr3 j
	 	   let k=a+1
	 	   writeArray arr3 j k
	 	   let j= mod (n*17) 10
	 	   a <- readArray arr4 j
	 	   let k=a+1
	 	   writeArray arr4 j k
	 	   modifyIORef i (+1)
       	 	   when (n/=(-1)) loop
        loop  -- start the first iteration 
       -- putStrLn "After the loop!"
        
	
	 
	
	putStrLn "Enter a number to find its frequency!"
	n <- getNumber
	let l=mod (n*5) 10
	min <- readArray arr1 l
	let m=mod (n*7) 10
	m2 <- readArray arr2 m
	let p=mod (n*13) 10
	m3 <- readArray arr3 p
	let o=mod (n*17) 10
	m4 <- readArray arr4 o
	putStrLn "Frequency is:!"
	let ab=
		if min>m2
	        	then m2
	      		else min
	
	let bc=
		if ab>m3
	        	then m3
	      		else ab
	
	let cd=
		if bc>m3
	        	then m4
	      		else bc

	print m4
	--putStrLn "Enter a number to find its frequency!"	
 	 --  let p =0
        --let loop= do
       	 --	   n <- getNumber
       	 --	   putStrLn "helo"
	 --	   writeArray arr1 n n
       	 	   
       	 --	   when (p<=10) loop
        --loop 		   
       -- print arr1
       -- print arr2
