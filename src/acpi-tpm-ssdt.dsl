DefinitionBlock (
    "acpi-tpm-ssdt.aml",// Output Filename
    "SSDT",             // Signature
    0x01,               // SSDT Compliance Revision
    "BXPC",             // OEMID
    "BXSSDT",           // TABLE ID
    0x1                 // OEM Revision
    )
{
    Scope(\_SB) {
        /* TPM with emulated TPM TIS interface */
        Device (TPM) {
            Name (_HID, EisaID ("PNP0C31"))
            Name (_CRS, ResourceTemplate ()
            {
                Memory32Fixed (ReadWrite, 0xFED40000, 0x00005000)
                //IRQNoFlags () {5}
            })
            Method (_STA, 0, NotSerialized) {
                Return (0x0F)
            }
        }
    }
}
