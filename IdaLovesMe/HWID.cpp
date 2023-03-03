#include "hwid.h"
#include "lazyimports.h"
#include "xorstr.h"
#include "sha1.hpp"

std::string c_hwid::get_hwid() {
	std::string fnl = "";

	HW_PROFILE_INFOA inf;
	if (LI_FN(GetCurrentHwProfileA).get()(&inf)) {
		fnl += inf.szHwProfileName;
		fnl += inf.szHwProfileGuid;
		fnl += std::to_string(inf.dwDockInfo);
	}

	int nIds_ = 0;
	int nExIds_ = 0;
	std::string vendor_ = "";
	std::string brand_ = "";
	bool isIntel_ = false;
	bool isAMD_ = false;
	std::bitset<32> f_1_ECX_;
	std::bitset<32> f_1_EDX_;
	std::bitset<32> f_7_EBX_;
	std::bitset<32> f_7_ECX_;
	std::bitset<32> f_81_ECX_;
	std::bitset<32> f_81_EDX_;
	std::vector<std::array<int, 4>> data_;
	std::vector<std::array<int, 4>> extdata_;
	std::array<int, 4> cpui;

	__cpuid(cpui.data(), 0);
	nIds_ = cpui[0];

	for (int i = 0; i <= nIds_; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		data_.push_back(cpui);
	}

	char vendor[0x20];
	memset(vendor, 0, sizeof(vendor));
	*reinterpret_cast<int*>(vendor) = data_[0][1];
	*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
	*reinterpret_cast<int*>(vendor + 8) = data_[0][2];
	vendor_ = vendor;

	__cpuid(cpui.data(), 0x80000000);
	nExIds_ = cpui[0];

	char brand[0x40];
	memset(brand, 0, sizeof(brand));

	for (int i = 0x80000000; i <= nExIds_; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		extdata_.push_back(cpui);
	}

	if (nExIds_ >= 0x80000001)
	{
		f_81_ECX_ = extdata_[1][2];
		f_81_EDX_ = extdata_[1][3];
	}

	if (nExIds_ >= 0x80000004)
	{
		memcpy(brand, extdata_[2].data(), sizeof(cpui));
		memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
		memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
		brand_ = brand;
	}

	fnl += vendor_;
	fnl += brand_;

	HANDLE hHdd = LI_FN(CreateFileA).get()(xorstr_("\\\\.\\PhysicalDrive0"), 0, 0, 0, OPEN_EXISTING, 0, 0);
	static STORAGE_PROPERTY_QUERY spq = { StorageDeviceProperty, PropertyStandardQuery };
	union {
		PVOID buf;
		PSTR psz;
		PSTORAGE_DEVICE_DESCRIPTOR psdd;
	};

	ULONG size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 0x100;
	ULONG dwError;

	do
	{
		dwError = ERROR_NO_SYSTEM_RESOURCES;

		if (buf = LI_FN(LocalAlloc).get()(0, size))
		{
			ULONG BytesReturned;

			if (LI_FN(DeviceIoControl).get()(hHdd, IOCTL_STORAGE_QUERY_PROPERTY, &spq, sizeof(spq), buf, size, &BytesReturned, 0))
			{
				if (psdd->Version >= sizeof(STORAGE_DEVICE_DESCRIPTOR))
				{
					if (psdd->Size > size)
					{
						size = psdd->Size;
						dwError = ERROR_MORE_DATA;
					}
					else {
						if (psdd->SerialNumberOffset) {
							fnl += (psz + psdd->SerialNumberOffset);
							break;
						}
					}
				}
				else
					dwError = ERROR_GEN_FAILURE;
			}
			else
				dwError = LI_FN(GetLastError).get()();

			LI_FN(LocalFree).get()(buf);
		}
	} while (dwError == ERROR_MORE_DATA);

	if (hHdd)
		LI_FN(CloseHandle).get()(hHdd);

	char hashed[41];
	sha1(fnl.c_str()).add(xorstr_("RnjVwTQqx3zRnA6Z")).print_hex(hashed);

	return hashed;
}