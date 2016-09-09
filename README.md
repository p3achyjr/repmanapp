This is the code powering Repman, as well as the companion app. Repman is a wristband that tracks your workout for you. Repman works by detecting the number of reps that you have completed, buzzing when you have finished your set, and buzzing again when you need to start the next set.

In this code, we wait until the start button has been pressed. After the start button has been pressed, we start looking for reps. Each time a rep has been completed, we increment a rep counter variable by one. Rep detection works by taking acceleration data, and detecting if there has been a peak or valley. This is motivated by the fact that doing a rep entails moving forward and backwards along a path. Thus, somewhere along that path, acceleration must reverse. After ten reps have been completed, Repman will buzz, and then sleep for 30 seconds (standard rest period for athletes). All variables related to rep detection will be reset. After 30 seconds, Repman will start waiting for the start button to be pressed, restarting the process.

The companion app connects to a set Bean in the background, and then allows the user to input exercise parameters. Upon pressing upload, the app sends a string to the Bean, which the Bean then reads.
