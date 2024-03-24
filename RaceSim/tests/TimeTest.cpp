#include <gtest/gtest.h>
#include "custom_time.h"
#include "units.h"


TEST(TimeTest, GetLocalReadableTime) {
    Time m_time = Time("2024-03-21 10:30:00", -9.5);
    EXPECT_EQ(m_time.get_local_readable_time(), "Thu Mar 21 10:30:00 2024");

    Time time = Time("2024-03-21 11:30:00");
    EXPECT_EQ(time.get_local_readable_time(), "2024:3:21");
}

TEST(TimeTest, UpdateSecondsUTCReadable){
    //Creates Time t for testing
    // -9.5 to convert from time in Alice Springs (which is 9.5 hours ahead of UTC)
    Time t = Time("2024-03-21 10:30:00", -9.5);

    //test update function
    t.update_time_seconds(14);

    //test both the local time and the UTC time
    EXPECT_EQ(t.get_local_readable_time(), "Thu Mar 21 10:30:14 2024");
    EXPECT_EQ(t.get_utc_readable_time(), "Thu Mar 21 01:00:14 2024");

    t.update_time_seconds(60);
    EXPECT_EQ(t.get_local_readable_time(), "Thu Mar 21 10:31:14 2024");
    EXPECT_EQ(t.get_utc_readable_time(), "Thu Mar 21 01:01:14 2024");

    t.update_time_seconds(5400);
    EXPECT_EQ(t.get_local_readable_time(), "Thu Mar 21 12:01:14 2024");
    EXPECT_EQ(t.get_utc_readable_time(), "Thu Mar 21 02:31:14 2024");
    
    t.update_time_seconds(43200);
    //test day update
    EXPECT_EQ(t.get_local_readable_time(), "Fri Mar 22 00:01:14 2024");
    EXPECT_EQ(t.get_utc_readable_time(), "Thu Mar 21 14:31:14 2024");

    Time time = Time("2023-12-31 23:30:00", -9.5);
    EXPECT_EQ(time.get_local_readable_time(), "Sun Dec 31 23:30:00 2023");
    EXPECT_EQ(time.get_utc_readable_time(), "Sun Dec 31 14:00:00 2023");

    time.update_time_seconds(5400);
    //test month and year change
    EXPECT_EQ(time.get_local_readable_time(), "Mon Jan 01 01:00:00 2024");
    EXPECT_EQ(time.get_utc_readable_time(), "Sun Dec 31 15:30:00 2023");
    
}


TEST(TimeTest, GetUTCtmAndTimePoint){
    Time t = Time("2024-03-21 10:30:14", -9.5);
    
    //verify all parts of tm
    EXPECT_EQ(t.get_utc_tm().tm_year, 124);
    EXPECT_EQ(t.get_utc_tm().tm_mon, 2);
    EXPECT_EQ(t.get_utc_tm().tm_mday, 21);
    EXPECT_EQ(t.get_utc_tm().tm_hour, 1);
    EXPECT_EQ(t.get_utc_tm().tm_min, 0);
    EXPECT_EQ(t.get_utc_tm().tm_sec, 14);
    EXPECT_EQ(t.get_utc_tm().tm_wday, 4);
    EXPECT_EQ(t.get_utc_tm().tm_yday, 80);
    EXPECT_EQ(t.get_utc_tm().tm_isdst, 0);

    //test the total number of seconds since  Jan 1, 1970 which is the timepoint value 
    EXPECT_EQ(t.get_utc_time_point(), 1710982814);

    Time time = Time("2023-12-31 23:30:00", -9.5);
    EXPECT_EQ(time.get_utc_tm().tm_year, 123);
    EXPECT_EQ(time.get_utc_tm().tm_mon, 11);
    EXPECT_EQ(time.get_utc_tm().tm_mday, 31);
    EXPECT_EQ(time.get_utc_tm().tm_hour, 14);
    EXPECT_EQ(time.get_utc_tm().tm_min, 0);
    EXPECT_EQ(time.get_utc_tm().tm_sec, 0);
    EXPECT_EQ(time.get_utc_tm().tm_wday, 0);
    EXPECT_EQ(time.get_utc_tm().tm_yday, 364);
    EXPECT_EQ(time.get_utc_tm().tm_isdst, 0);

    EXPECT_EQ(time.get_utc_time_point(), 1704031200);


}