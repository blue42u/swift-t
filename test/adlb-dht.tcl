
# Flex ADLB DHT

package require turbine 0.1
adlb::init 1
turbine_init

set count 4

if { ! [ adlb::amserver ] } {
    set rank [ adlb::rank ]
    puts "rank: $rank"
    set workers [ adlb::workers ]

    for { set i 0 } { $i < $count } { incr i } {
        set id [ expr ( $rank + $i ) % $workers ]
        adlb::store $id "msg rank:$rank"

        set id [ expr ( $rank + 1 + $i ) % $workers ]
        set msg [ adlb::retrieve $id ]
        puts "got: $msg"
    }
}

turbine_finalize
adlb::finalize
puts OK
