#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../DeviceDriver/FlashMemoryDevice.h"
#include "../DeviceDriver/DeviceDriver.cpp"

using namespace std;
using namespace testing;

// 목클래스
class FlashMock : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DriverFixture : public Test {
public:
	FlashMock mk;
};

TEST_F(DriverFixture, TestReadFiveTimes) {
	EXPECT_CALL(mk, read(0xA)).Times(5);

	DeviceDriver* dd = new DeviceDriver(&mk);
	dd->read(0xA);
}

TEST_F(DriverFixture, TestReadFiveTimesDiffValue) {
	EXPECT_CALL(mk, read(0xA))
		.Times(5)
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillOnce(Return(2))
		.WillRepeatedly(Return(1));

	DeviceDriver* dd = new DeviceDriver(&mk);

	EXPECT_THROW(dd->read(0xA), ReadFailException);
}

TEST_F(DriverFixture, TestWriteOnce) {
	EXPECT_CALL(mk, read(0xA))
		.Times(1)
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mk, write(0xA, 1)).Times(1);

	DeviceDriver* dd = new DeviceDriver(&mk);
	dd->write(0xA, 1);
}


TEST_F(DriverFixture, TestWriteFail) {
	EXPECT_CALL(mk, read(0xA))
		.WillRepeatedly(Return(0x01));

	DeviceDriver* dd = new DeviceDriver(&mk);
	EXPECT_THROW(dd->write(0xA, 1), WriteFailException);
}
