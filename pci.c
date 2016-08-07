#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciRootBridgeIo.h>

#define HEX_KEY_INPUT( x ) ( ( (x <= '9') && (x >= '0') ) || ( (x <= 'F') && (x >= 'A') ) || ( (x <= 'f') && (x >= 'a') ) )
#define CALC_EFI_PCI_ADDRESS( Bus, Dev, Fun, Reg ) \
	( (UINT64) ( ( ( (UINTN) Bus) << 24) + ( ( (UINTN) Dev) << 16) + \
		     ( ( (UINTN) Fun) << 8) + ( (UINTN) Reg) ) )

#define SCAN_CODE_NULL		0x0000
#define SCAN_CODE_UP		0x0001
#define SCAN_CODE_DOWN		0x0002
#define SCAN_CODE_RIGHT		0x0003
#define SCAN_CODE_LEFT		0x0004
#define SCAN_CODE_PAGE_UP	0x0009
#define SCAN_CODE_PAGE_DOWN	0x000A
#define SCAN_CODE_ESC		0x0017
#define SCAN_CODE_F1		0x000B
#define CHAR_CR			0x000D

EFI_STATUS PCI_Reg( UINT8, UINT8, UINT8 );


UINTN
Xtoi(
	IN CHAR16  *Str
	)
{
	return(StrHexToUintn( Str ) );
}


EFI_STATUS
EFIAPI
UefiMain
	( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE  *SystemTable )
{
	EFI_INPUT_KEY	Key;
	EFI_STATUS	Status;
	UINT8		Addr;
	UINT8		Bus;
	UINT8		Dev;
	UINT8		Fun;

	UINTN	Index3;
	UINT8	InPutKeyFlag;
	CHAR16	*InPutKey;
	InPutKey = AllocatePool( 2 * 9 );


	Status		= EFI_SUCCESS;
	InPutKeyFlag	= 0;
	Bus		= 0;
	Dev		= 0;
	Fun		= 0;
	Addr		= 0;

	gST->ConOut->ClearScreen( gST->ConOut );
	Print( L"Bus:00" );
	for (;; )
	{
		gST->ConOut->EnableCursor( gST->ConOut, TRUE );
		gST->ConOut->SetCursorPosition( gST->ConOut, 4, 0 );
		Status	= gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index3 );
		Status	= gST->ConIn->ReadKeyStroke( gST->ConIn, &Key );

		if ( HEX_KEY_INPUT( Key.UnicodeChar ) )
		{
			if ( InPutKeyFlag >= 2 )
				InPutKeyFlag = 0;

			InPutKey[InPutKeyFlag]		= Key.UnicodeChar;
			InPutKey[InPutKeyFlag + 1]	= L'\0';
			InPutKeyFlag++;

			if ( InPutKeyFlag != 0 )
			{
				Addr = (UINT8) Xtoi( InPutKey );
				if ( Addr < 0x10 )
				{
					Print( L"0%x", Addr );
				} else {
					Print( L"%x", Addr );
				}
			}

			continue;
		}

		if ( Key.UnicodeChar == CHAR_CR )
		{
			Bus		= Addr;
			InPutKeyFlag	= 0;
			Addr		= 0;
			break;
		}

		if ( Key.ScanCode == SCAN_CODE_ESC )
		{
			break;
		}
	}

	if ( Bus < 0x10 )
	{
		Print( L"0%x", Bus );
	} else { Print( L"%x", Bus ); }
	Print( L" " );
	Print( L"Dev:00" );
	for (;; )
	{
		gST->ConOut->EnableCursor( gST->ConOut, TRUE );
		gST->ConOut->SetCursorPosition( gST->ConOut, 11, 0 );
		Status	= gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index3 );
		Status	= gST->ConIn->ReadKeyStroke( gST->ConIn, &Key );

		if ( HEX_KEY_INPUT( Key.UnicodeChar ) )
		{
			if ( InPutKeyFlag >= 2 )
				InPutKeyFlag = 0;

			InPutKey[InPutKeyFlag]		= Key.UnicodeChar;
			InPutKey[InPutKeyFlag + 1]	= L'\0';
			InPutKeyFlag++;

			if ( InPutKeyFlag != 0 )
			{
				Addr = (UINT8) Xtoi( InPutKey );
				if ( Addr < 0x10 )
				{
					Print( L"0%x", Addr );
				} else {
					Print( L"%x", Addr );
				}
			}

			continue;
		}
		if ( Key.UnicodeChar == CHAR_CR )
		{
			Dev		= Addr;
			InPutKeyFlag	= 0;
			Addr		= 0;

			break;
		}

		if ( Key.ScanCode == SCAN_CODE_ESC )
		{
			break;
		}
	}

	if ( Dev < 0x10 )
	{
		Print( L"0%x", Dev );
	} else { Print( L"%x", Dev ); }
	Print( L" " );
	Print( L"Fun:00" );
	for (;; )
	{
		gST->ConOut->EnableCursor( gST->ConOut, TRUE );
		gST->ConOut->SetCursorPosition( gST->ConOut, 18, 0 );
		Status	= gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index3 );
		Status	= gST->ConIn->ReadKeyStroke( gST->ConIn, &Key );

		if ( HEX_KEY_INPUT( Key.UnicodeChar ) )
		{
			if ( InPutKeyFlag >= 2 )
				InPutKeyFlag = 0;

			InPutKey[InPutKeyFlag]		= Key.UnicodeChar;
			InPutKey[InPutKeyFlag + 1]	= L'\0';
			InPutKeyFlag++;

			if ( InPutKeyFlag != 0 )
			{
				Addr = (UINT8) Xtoi( InPutKey );
				if ( Addr < 0x10 )
				{
					Print( L"0%x", Addr );
				} else {
					Print( L"%x", Addr );
				}
			}

			continue;
		}

		if ( Key.UnicodeChar == CHAR_CR )
		{
			Fun		= Addr;
			InPutKeyFlag	= 0;
			Addr		= 0;


			PCI_Reg( Bus, Dev, Fun );


			break;
		}

		if ( Key.ScanCode == SCAN_CODE_ESC )
		{
			break;
		}
	}


	Status = EFI_SUCCESS;
	gST->ConOut->ClearScreen( gST->ConOut );
	return(Status);
}


EFI_STATUS
PCI_Reg( UINT8 Bus, UINT8 Dev, UINT8 Fun )
{
	EFI_INPUT_KEY			Key;
	EFI_STATUS			Status;
	EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciIo = NULL;

	UINT8	Reg8;
	UINT16	Reg16;
	UINT64	Address;
	UINT8	Index;
	UINT8	Index2;
	UINT8	Index_w;
	UINTN	Index3;


	UINT8	Reg_Select1;
	UINT8	Reg_Select2;
	UINT8	InPutKeyFlag;
	CHAR16	*InPutKey;
	InPutKey = AllocatePool( 2 * 9 );
	Status = EFI_SUCCESS;

	Reg16	= 0;
	Reg8	= 0;
	Address = 0;
	Reg_Select1	= 3;
	Reg_Select2	= 4;
	InPutKeyFlag	= 0;


	Index2 = 0;
	gST->ConOut->ClearScreen( gST->ConOut );
	Status = gBS->LocateProtocol(
		&gEfiPciRootBridgeIoProtocolGuid,
		NULL,
		(VOID * *) &PciIo
		);


	if ( EFI_ERROR( Status ) )
	{
		Print( L"Status=%x\n", Status );
		Print( L"Error!\n" );
	}else   {
		Print( L"Bus:" );
		if ( Bus < 0x10 )
		{
			Print( L"0%x", Bus );
		} else { Print( L"%x", Bus ); } Print( L" " );
		Print( L"Dev:" );
		if ( Dev < 0x10 )
		{
			Print( L"0%x", Dev );
		} else { Print( L"%x", Dev ); } Print( L" " );
		Print( L"Fun:" );
		if ( Fun < 0x10 )
		{
			Print( L"0%x", Fun );
		} else { Print( L"%x", Fun ); } Print( L"\n" );
		Address = CALC_EFI_PCI_ADDRESS( Bus, Dev, Fun, 0 );

		Status = PciIo->Pci.Read( PciIo,
					  EfiPciWidthUint16,
					  Address,
					  1,
					  &Reg16
					  );
		Print( L"Vendor ID = %x\n", Reg16 );
		Address = CALC_EFI_PCI_ADDRESS( Bus, Dev, Fun, 0 );

		Status = PciIo->Pci.Read( PciIo,
					  EfiPciWidthUint16,
					  Address + 2,
					  1,
					  &Reg16
					  );
		Print( L"Device ID = %x\n", Reg16 );
		Print( L"   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n" );
		for ( Index = 0; Index <= 0xFF; Index++ )
		{
			Address = CALC_EFI_PCI_ADDRESS( Bus, Dev, Fun, 0 );

			Status = PciIo->Pci.Read( PciIo,
						  EfiPciWidthUint8,
						  Address + Index,
						  1,
						  &Reg8
						  );
			if ( (Index % 0x10) == 0 )
			{
				Print( L"0%x ", Index2 );
				Index2++;
			}
			if ( Reg8 < 0x10 )
			{
				Print( L"0%x", Reg8 );
			}else                          {
				Print( L"%x", Reg8 );
			}
			Print( L" " );
			if ( (Index % 0x10) == 0x0F )
				Print( L"\n" );
			if ( Index == 0xFF )
				break;
		}
	}

	for (;; )
	{
		gST->ConOut->EnableCursor( gST->ConOut, TRUE );
		if ( Reg_Select1 > 16 * 3 )
			Reg_Select1 = 3;
		if ( Reg_Select1 < 3 )
			Reg_Select1 = 16 * 3;
		if ( Reg_Select2 > (15 + 4) )
			Reg_Select2 = 4;
		if ( Reg_Select2 < 4 )
			Reg_Select2 = 19;
		Index_w = (Reg_Select2 - 4) * 16 + (Reg_Select1 / 3) - 1;
		gST->ConOut->SetCursorPosition( gST->ConOut, Reg_Select1, Reg_Select2 );
		Status	= gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index3 );
		Status	= gST->ConIn->ReadKeyStroke( gST->ConIn, &Key );


		/* pressed UP */
		if ( Key.ScanCode == SCAN_CODE_UP )
		{
			InPutKeyFlag = 0;
			{ Reg_Select2 -= 1; }

			continue;
		}
		/* pressed DOWN */
		if ( Key.ScanCode == SCAN_CODE_DOWN )
		{
			InPutKeyFlag = 0;
			{ Reg_Select2 += 1; }

			continue;
		}
		/* pressed LEFT */
		if ( Key.ScanCode == SCAN_CODE_LEFT )
		{
			InPutKeyFlag	= 0;
			Reg_Select1	-= 3;
			continue;
		}
		/* pressed RIGHT */
		if ( Key.ScanCode == SCAN_CODE_RIGHT )
		{
			InPutKeyFlag	= 0;
			Reg_Select1	+= 3;
			continue;
		}


		if ( Key.UnicodeChar == CHAR_CR )
		{
			if ( InPutKeyFlag != 0 )
			{
				{ Reg8 = (UINT8) Xtoi( InPutKey );

				  PciIo->Pci.Write( PciIo, EfiPciWidthUint8, Address + Index_w, 1, &Reg8 ); /* write data to pci config space */
				}
				InPutKeyFlag = 0;
			}

			{
				Index2 = 0;
				gST->ConOut->ClearScreen( gST->ConOut );
				Print( L"Bus:" );
				if ( Bus < 0x10 )
				{
					Print( L"0%x", Bus );
				} else { Print( L"%x", Bus ); } Print( L" " );
				Print( L"Dev:" );
				if ( Dev < 0x10 )
				{
					Print( L"0%x", Dev );
				} else { Print( L"%x", Dev ); } Print( L" " );
				Print( L"Fun:" );
				if ( Fun < 0x10 )
				{
					Print( L"0%x", Fun );
				} else { Print( L"%x", Fun ); } Print( L"\n" );
				Address = CALC_EFI_PCI_ADDRESS( Bus, Dev, Fun, 0 );

				Status = PciIo->Pci.Read( PciIo,
							  EfiPciWidthUint16,
							  Address,
							  1,
							  &Reg16
							  );
				Print( L"Vendor ID = %x\n", Reg16 );
				Address = CALC_EFI_PCI_ADDRESS( Bus, Dev, Fun, 0 );

				Status = PciIo->Pci.Read( PciIo,
							  EfiPciWidthUint16,
							  Address + 2,
							  1,
							  &Reg16
							  );
				Print( L"Device ID = %x\n", Reg16 );
				Print( L"   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n" );
				for ( Index = 0; Index <= 0xFF; Index++ )
				{
					Address = CALC_EFI_PCI_ADDRESS( Bus, Dev, Fun, 0 );

					Status = PciIo->Pci.Read( PciIo,
								  EfiPciWidthUint8,
								  Address + Index,
								  1,
								  &Reg8
								  );
					if ( (Index % 0x10) == 0 )
					{
						Print( L"0%x ", Index2 );
						Index2++;
					}
					if ( Reg8 < 0x10 )
					{
						Print( L"0%x", Reg8 );
					}else                          {
						Print( L"%x", Reg8 );
					}
					Print( L" " );
					if ( (Index % 0x10) == 0x0F )
						Print( L"\n" );
					if ( Index == 0xFF )
						break;
				}
			}

			continue;
		}
		if ( HEX_KEY_INPUT( Key.UnicodeChar ) )
		{
			if ( InPutKeyFlag >= 2 )
				InPutKeyFlag = 0;

			InPutKey[InPutKeyFlag]		= Key.UnicodeChar;
			InPutKey[InPutKeyFlag + 1]	= L'\0';
			InPutKeyFlag++;

			if ( InPutKeyFlag != 0 )
			{
				Reg8 = (UINT8) Xtoi( InPutKey );
				if ( Reg8 < 0x10 )
				{
					Print( L"0%x", Reg8 );
				} else {
					Print( L"%x", Reg8 );
				}
				/* Print(L"%d",InPutKeyFlag); */
			}

			continue;
		}


		if ( Key.ScanCode == SCAN_CODE_ESC )
		{
			break;
		}
	}
	Status = EFI_SUCCESS;
	gST->ConOut->ClearScreen( gST->ConOut );
	FreePool( InPutKey );
	return(Status);
}


/*
 * IORead32(0x0cfc);
 * IOWrite32(0x0cf8,address)
 */