# this experiment depends on mat201b2013 #

    cd mat201b2013
    git clone git@spectrum.mat.ucsb.edu:app_with_state.git
    cd app_with_state
    ./copy_app_with
    cd ..
    ./buildandrun.sh app_with_state/test.cpp
