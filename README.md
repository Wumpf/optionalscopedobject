# optionalscopedobject

Recently I ran into a weird situation where a threading lock was either passed, or not (since locking might or might not be appropriate in a given situation).
Since I am always very concerned about other people adding early out return statements and forgetting to do the cleanup (here, unlocking the lock), I want to use a scoped version of the lock. But having the scope optional forced me into using an allocation:
 ``` cpp
void somethingProbablyUnsafe(ThreadingLock* lock)
{
  // unrelated code
  // ...
  
	std::unique_ptr<ScopedLock> scopedLock;
	if(lock)
	{
		scopedLock.reset(new ScopedLock(*lock));
	}

	// Complicated code with early outs.
	// ...
}
 ```
This certainly solves the problem - the lock is still scoped and nobody needs to worry about having the lock unlocked. Way better than checking the lock variable at each return point. But it requires an allocation and no longer keeps the ScopedLock on the stack as it is intended to be, how awful! :(

The little template class in this repository solves this problem at the overhead of a single bool. Usage:
 ``` cpp
void somethingProbablyUnsafe(ThreadingLock* lock)
{
  // unrelated code
  // ...
  
	OptionalScopedObject<ScopedLock> scopedLock;
	if(lock)
	{
		scopedLock.construct(*lock);
	}

	// Complicated code with early outs.
	// ...
}
 ```
 Scoped thread locks are of course only an example. The template can be used with any kind of scoped object that may or may not be required.
