{-# LANGUAGE DatatypeContexts #-}


import qualified Data.List
import Data.Array.IO
import Data.List 
import System.IO  
import Control.Monad
import Control.Concurrent
import System.Posix.Process
import System.Process
import Data.IORef
import Data.Typeable
import qualified Data.ByteString as Str
import System.Environment (getArgs)
import qualified Control.Exception as Exc
import System.Posix.Unistd

data SplayTree s  = 
  SplayTree s (SplayTree s) (SplayTree s)
  | Leaf


find_index list key = [ (index+1,list !! index) | index <-[0..(length(list)-1)] , (key == (list!!index))]	


seek :: (Ord s) => s-> SplayTree s-> SplayTree s 
seek _ Leaf = Leaf
seek serach_node copy_splay@(SplayTree current_node left_sub right_sub) =

  if (serach_node == current_node) then copy_splay

  else if (serach_node < current_node) then

    	case seek serach_node left_sub of
     	 Leaf -> copy_splay
     	 left_tree -> right_rotate left_tree copy_splay
       else 
   	 case seek serach_node right_sub of
    	  Leaf -> copy_splay
    	  right_tree -> left_rotate copy_splay right_tree




right_rotate :: (Ord s) => SplayTree s-> SplayTree s-> SplayTree s 
right_rotate (SplayTree key1 left_sub1 right_sub1) (SplayTree key2 left_sub2 right_sub2) =
			SplayTree key1 left_sub1 (SplayTree key2  right_sub1 right_sub2)


left_rotate :: (Ord s) => SplayTree s-> SplayTree s-> SplayTree s 
left_rotate (SplayTree key1 left_sub1 right_sub1) (SplayTree key2 left_sub2 right_sub2) =
  			SplayTree key2  (SplayTree key1 left_sub1 left_sub2) right_sub2




insert_splay :: (Ord s) => s -> SplayTree s -> SplayTree s 
insert_splay insert_key  splay =
  case (seek insert_key splay) of
    Leaf -> (SplayTree insert_key Leaf Leaf)
    modifed_tree@(SplayTree current_root left right) 
        | current_root < insert_key ->
		 SplayTree insert_key  (SplayTree current_root left Leaf) right
        |otherwise ->
		 SplayTree insert_key  left (SplayTree current_root Leaf right)



root :: SplayTree s -> s
root Leaf = error "root of empty tree"
root (SplayTree s left right) = s

rightchild :: (Ord s) => SplayTree s -> SplayTree s
rightchild (SplayTree s left right) = right

leftchild :: (Ord s) => SplayTree s -> SplayTree s
leftchild t@(SplayTree s left right) = left






splay_right :: (Ord s) => SplayTree s -> SplayTree s 
splay_right Leaf = Leaf

splay_right ret_basecase@(SplayTree root left Leaf) = ret_basecase

splay_right (SplayTree root_parent left_sub (SplayTree right_sub_root left_sub_of_right right_sub_of_right)) = 
  splay_right (SplayTree right_sub_root  (SplayTree root_parent left_sub left_sub_of_right) right_sub_of_right)


splay_left :: (Ord k) => SplayTree k -> SplayTree k 
splay_left Leaf = Leaf

splay_left ret_basecase@(SplayTree  root Leaf right) = ret_basecase

splay_left (SplayTree root_parent (SplayTree left_sub_root  left_sub_of_left right_sub_of_left) right_sub) = 
  splay_left (SplayTree left_sub_root left_sub_of_left (SplayTree root_parent  right_sub_of_left right_sub))


list_to_splay :: (Ord s) => [s] -> SplayTree s 
list_to_splay [] = Leaf
list_to_splay l = foldl (\ ac(s)-> insert_splay s ac) Leaf l





f1 :: [String] -> [Int]
f1 = map read

f' :: [Int] -> [String]
f' = (>>= return.show)

f2 :: [String] -> [String]
f2 = map read

getNumber::IO Int
getNumber = do
    num <- getLine
    return (read num)



merge :: (Ord a) => [a] -> [a] -> [a]
merge list [] = list
merge [] list = list
merge (headleft:leftlist) (headright:rightlist) = if (headleft>headright) 
						  then headright:merge (headleft:leftlist) rightlist
    		      				  else headleft:merge leftlist (headright:rightlist) 


mergesort :: (Ord a) => [a] -> [a]
mergesort []=[]
mergesort list = if  (length list) > 1 
	       	 then merge (mergesort leftlist) (mergesort rightlist)
   	       	 else list
    			where (leftlist,rightlist) = (take mid list, drop mid list)
					where mid = (length list) `div` 2 

ms::(Ord a,Show a)=>([a],IORef Int)->IO()
ms(a,globl)=do
	let b=mergesort(a)
	out<-readIORef globl
	if out/=1
	   then do 
	   	modifyIORef globl (+1)
		print b
	   else
		putStrLn  ""			
	
	--putStrLn ""

quicksort :: (Ord a) => [a] -> [a]
quicksort [] = []
quicksort (first:rest) = quicksort smaller ++ mid ++ quicksort larger
				where        smaller = [a | a<-rest, a<first]
 					     larger = [c | c<-rest, c>first]
					     mid =   [b | b<-rest, b==first] ++ [first]

qs::(Ord a,Show a)=>([a],IORef Int)-> IO()
qs(a,globl)=do
	
	let b=quicksort(a)
	out<-readIORef globl
	if out/=1
	   then do 
	   	modifyIORef globl (+1)
		print b
	   else
		putStrLn  ""			
	
	
add_element :: (Ord a) => a -> [a]  -> [a]
add_element m [] = [m]
add_element x xs = x:xs

containing_sub  list key = [ (i+1,list !! i) | i<-[0..(length(list)-1)] , (sub_string key (list!!i))]	

sub_string :: String -> String -> Bool
sub_string (xhead:xtail) [] = False
sub_string x_to_search in_y =
    if (match x_to_search in_y == True) then True
    else if (sub_string x_to_search (tail in_y) == True) then True
	else
    		False

match :: String -> String -> Bool
match [] in_y = True
match (xhead:xtail) [] = False
match (xhead:xtail) (yhead:ytail) = (xhead==yhead) && match xtail ytail

						
head_match :: String -> String -> Bool
head_match [] y = True
head_match (x:xs) [] = False
head_match (x:xs) (y:ys) = (x == y) && head_match xs ys

						
start_string  list key = [ (i+1,list !! i) | i<-[0..(length(list)-1)] , (head_match key (list!!i))]	


strlen_search  list key = [ (i+1,list !! i) | i<-[0..(length(list)-1)] , key==length(list!!i)]

	
delete_from_list :: Eq a => a -> [a] -> [a]
delete_from_list deleted xs = [ xs!!i | i <- [0..(length (xs)-1)], (is_equal deleted (xs!!i))==False ]	

				
is_equal ::Eq a => a -> a -> Bool
is_equal key1 key2 = 
	if key1==key2 then True
		else False


forceList::[a]->()
forceList [] = ()
forceList (head:tail) = forceList tail


main = do


	putStrLn "\n\nEnter the file name fron which data is to be read";
	--filename <- getLine;
	--putStrLn ("\n\n")

	let list_main = []
	let list_main2 = []


        
	let modified_list=[]

	x<- newIORef (0::Int)
        globl<-newIORef(0::Int)
	let loop= do
		

		    putStrLn "Press 1 to search for a query\n";
		    putStrLn "Press 2 to search a query using substring/stringlength matching\n";
		    putStrLn "Press 3 to search for indices of the query in file\n";
		    putStrLn "Press 4 to delete some query from history\n";
		    putStrLn "Press 5 to to sort the input data in ascending order of magnitude\n";
		    putStrLn "Press 6 to estimate rough frequency of the query \n";	
	            putStrLn "Press 7 to exit\n";


		    handle <- openFile "todo.txt" ReadMode
                    --handle <- openFile filename ReadMode
                    contents <- hGetContents handle

                    let singlewords = words contents

                   	list_main = f1 singlewords

		    forceList contents `seq` hClose handle
		    
		    choice <- getNumber;

		    
			   if choice==1
	        		then do
					putStrLn ("Enter the query\n")
					search_query <-getNumber
					print search_query	

					let splaytree= list_to_splay list_main;
					let modified_tree= splaytree
					let modified_list=add_element search_query list_main

					modifyIORef x(+1)
					oldValue <- readIORef x
	
					readIORef x >>=print

					putStrLn ("Maybe you are searching for one of the following queries\n")

					
					let z1 = Exc.catch (print $ root (modified_tree)) handler
   						where
        						handler :: Exc.ErrorCall -> IO ()
        						handler _ = putStrLn $ "Null";
					z1

					let z2 = Exc.catch (print $ root (rightchild(modified_tree))) handler
   						where
        						handler :: Exc.ErrorCall -> IO ()
        						handler _ = putStrLn $ "Null";

					z2

					let z3 = Exc.catch (print $ root (leftchild(modified_tree))) handler
   						where
        						handler :: Exc.ErrorCall -> IO ()
        						handler _ = putStrLn $ "Null";

					z3

					let z4 = Exc.catch (print $ root (rightchild(leftchild(modified_tree)))) handler
   						where
        						handler :: Exc.ErrorCall -> IO ()
        						handler _ = putStrLn $ "Null";

					z4


					let z5 = Exc.catch (print $ root (leftchild (rightchild(modified_tree)))) handler
   						where
        						handler :: Exc.ErrorCall -> IO ()
        						handler _ = putStrLn $ "Null"
					
					z5

					let z6 = Exc.catch (print $ root (leftchild(leftchild(modified_tree)))) handler
   						where
        						handler :: Exc.ErrorCall -> IO ()
        						handler _ = putStrLn $ "Null"
					
					z6

					let z7 = Exc.catch (print $ root (rightchild (rightchild(modified_tree)))) handler
   						where
        						handler :: Exc.ErrorCall -> IO ()
        						handler _ = putStrLn $ "Null"
					
					z7


				        let lists=f' modified_list	
					outh <- openFile "todo.txt" WriteMode
					hPutStrLn outh (unlines lists)
    					hClose outh


					
	      			else putStrLn ("");

		    
			   if choice==2
				then do

				        handle2 <- openFile "todo.txt" ReadMode
				        --handle <- openFile filename ReadMode
				        contents2 <- hGetContents handle2
					let list_main2 = words contents2
					
					putStrLn ("Press 'a' to search for queries containing a substring\n")
					putStrLn ("Press 'b' to search for queries starting with substring\n")
					putStrLn ("Press 'c' to search for queries of particular string length\n")
					
					sub_choice <- getLine; 
						
						if (sub_choice=="a")
							then do				
								putStrLn ("Enter the substring to search for\n")
								get_index <- getLine
								let indices_substring =containing_sub list_main2 get_index
								putStrLn ("You might want to search for");
								mapM_ print (indices_substring)

						else if (sub_choice=="b")
							then do				
								putStrLn ("Enter the substring to search for\n")
								get_index <- getLine
								let indices_substring =start_string list_main2 get_index
								putStrLn ("You might want to search for");
								mapM_ print (indices_substring)

						else if (sub_choice=="c")
							then do				
								putStrLn ("Enter the stringlength to search for\n")
								get_index <- getNumber
								let indices_strlen =strlen_search list_main2 get_index
								putStrLn ("You might want to search for");
								mapM_ print (indices_strlen)
							else
								putStrLn ("");
							
					                forceList contents `seq` hClose handle2


				else putStrLn ("") ;

		    
			   if choice==3
				then do
					putStrLn ("Enter the query of which index is to be found\n")
					get_index <- getNumber
					let indices=find_index list_main (get_index)
					print ("The line number in files matching the query are\n")
					mapM_ print (indices)

				else putStrLn ("") ;

			   if choice==4
				then do
					putStrLn ("Enter the query to be removed fromt the history\n")
					get_index <- getNumber
					let new_list= delete_from_list (get_index) list_main 
					let lists=f' new_list
							--putStrLn "mai chala"
					outh <- openFile "todo.txt" WriteMode
					hPutStrLn outh (unlines lists)
    					hClose outh
							--appendFile "to.txt" "\n"
					print ("Succesfully deleted\n")

				else putStrLn ("") ;

		    

		    
			   if choice==5
				then do
					print ("Sorted sequence is\n")					
					modifyIORef globl (0*)
					forkIO (ms(list_main,globl))	
					forkIO (qs(list_main,globl))				
					sleep(1)
					putStrLn ""
				else putStrLn (""); --print (quicksort (list))

		    
		    
			   if choice==6
	        		then do
				forceList contents `seq` hClose handle
	        		callProcess "./d" []				
				print ("Frequency estimator")
		
	      			else putStrLn ("");

			   if (choice>=8 || choice<=0)
	        		then print ("Invalid input\n")
	      			else putStrLn ("");

			    

       	 	    when (choice/=(7)) loop

				
        loop  
