valenti@ValentiUbuntu:~/Development/Embedded/gl_embeddedlinux/build-exercise4-Desktop_Qt_5_15_1_GCC_64bit-Debug$ valgrind --tool=memcheck --leak-check=yes ./game_app -n 5 -p 3 -v 3
==83835== Memcheck, a memory error detector
==83835== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==83835== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==83835== Command: ./game_app -n 5 -p 3 -v 3
==83835== 
game setup: iterations:3, num_dices: 5
Player ID: 1, result:51 
Player ID: 2, result:52 
Player ID: 3, result:49 
History of player with ID: 1 
[2][5][4][2][6][5][1][1][5][5][6][1][1][1][6]
 END HISTORY 
History of player with ID: 2 
[2][5][1][4][2][6][3][4][4][3][4][2][5][2][5]
 END HISTORY 
History of player with ID: 3 
[3][2][3][2][6][3][3][2][2][2][4][4][4][6][3]
 END HISTORY 
==83835== 
==83835== HEAP SUMMARY:
==83835==     in use at exit: 0 bytes in 0 blocks
==83835==   total heap usage: 49 allocs, 49 frees, 2,224 bytes allocated
==83835== 
==83835== All heap blocks were freed -- no leaks are possible
==83835== 
==83835== For lists of detected and suppressed errors, rerun with: -s
==83835== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
valenti@ValentiUbuntu:~/Development/Embedded/gl_embeddedlinux/build-exercise4-Desktop_Qt_5_15_1_GCC_64bit-Debug$ valgrind --tool=memcheck --leak-check=yes ./game_app -n 5 -p 3 -v 3
==83836== Memcheck, a memory error detector
==83836== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==83836== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==83836== Command: ./game_app -n 5 -p 3 -v 3
==83836== 
game setup: iterations:3, num_dices: 5
Player ID: 1, result:51 
Player ID: 2, result:52 
Player ID: 3, result:49 
History of player with ID: 1 
[2][5][4][2][6][5][1][1][5][5][6][1][1][1][6]
 END HISTORY 
History of player with ID: 2 
[2][5][1][4][2][6][3][4][4][3][4][2][5][2][5]
 END HISTORY 
History of player with ID: 3 
[3][2][3][2][6][3][3][2][2][2][4][4][4][6][3]
 END HISTORY 
==83836== 
==83836== HEAP SUMMARY:
==83836==     in use at exit: 0 bytes in 0 blocks
==83836==   total heap usage: 49 allocs, 49 frees, 2,224 bytes allocated
==83836== 
==83836== All heap blocks were freed -- no leaks are possible
==83836== 
==83836== For lists of detected and suppressed errors, rerun with: -s
==83836== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
valenti@ValentiUbuntu:~/Development/Embedded/gl_embeddedlinux/build-exercise4-Desktop_Qt_5_15_1_GCC_64bit-Debug$ 
