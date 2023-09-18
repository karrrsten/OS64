#pragma once

#include "acpi_tables.h"

/**
 * @brief Locate a certain ACPI Table.
 * @param signatur The 4-byte signature of the table to locate.
 * @return The ACPI table or nullptr if it could not be found.
 * @see See acpi_tables.h for some possible ACPI tables and their signaturs.
 */
void *acpi_get_table(const char signature[static 4]);
