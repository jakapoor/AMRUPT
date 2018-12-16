## Indoor Testing 12/15/2018

Indoor tests were performed as described in the Indoor_Testing_Protocol document. Pictures of the setup are also posted.

When performing test 1, a phase shift occurs 1-2 seconds after switching from the noise source to the antenna inputs. The [video](https://www.youtube.com/watch?v=EleJ9V3Auck) of this can be seen here. Test 2 exhibited the same results as test 1 for the same reason.

In test 3, there was no phase shift because there was no RF switching in this test and the end result was 90 degrees for each trial. Test 4 exhibited the same results as test 3 for the same reason.

Test 5 shows the resiliency of the CC1310 noise for both sample offset and phase offset synchronization (the sinusoid yielded the same phase difference as the post-calibration noise).

Test 6 yielded results that were offsetted from 90 degrees due to the noise to antenna switch. However, results from test 7 exhibit no offset because phase synchronization was performed after the quick phase shift when switching from the noise source to the antenna inputs. 

Results from test 7 indicate that the noise source can still be used for sample offset synchronization if phase synchronization is performed sometime after the RF switch. Also, I do not think that the phase shift from the noise to antenna switch is the same phase shift as the one exhibited during the outdoor tests. This is because the outdoor bimodal phase shift occurs more than 5 seconds after the RF switch and is more random. I believe the phase shift from the RF switch and the outdoor bimodal phase shift were compounded together during the last round of outdoor testing.
