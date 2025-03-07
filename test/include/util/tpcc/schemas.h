#pragma once

#include <vector>
#include "catalog/schema.h"
#include "common/macros.h"
#include "storage/index/index_defs.h"

namespace terrier::tpcc {

/**
 * Schemas is a utility class that defines all 9 table schemas and 8 index schemas according to section 1.3 of the TPC-C
 * specification. It also defines 2 secondary indexes that improve query performance.
 */
struct Schemas {
  Schemas() = delete;

  /**
   * @param oid_counter global OID counter to be incremented
   * @return Warehouse table schema
   */
  static catalog::Schema BuildWarehouseTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> warehouse_columns;
    warehouse_columns.reserve(num_warehouse_table_cols_);

    // 2*W unique IDs
    warehouse_columns.emplace_back("W_ID", type::TypeId::TINYINT, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 10
    warehouse_columns.emplace_back("W_NAME", type::TypeId::VARCHAR, 10, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    warehouse_columns.emplace_back("W_STREET_1", type::TypeId::VARCHAR, 20, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    warehouse_columns.emplace_back("W_STREET_2", type::TypeId::VARCHAR, 20, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    warehouse_columns.emplace_back("W_CITY", type::TypeId::VARCHAR, 20, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 2
    warehouse_columns.emplace_back("W_STATE", type::TypeId::VARCHAR, 2, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 9
    warehouse_columns.emplace_back("W_ZIP", type::TypeId::VARCHAR, 9, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(4,4)
    warehouse_columns.emplace_back("W_TAX", type::TypeId::DECIMAL, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(12,2)
    warehouse_columns.emplace_back("W_YTD", type::TypeId::DECIMAL, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(warehouse_columns.size() == num_warehouse_table_cols_,
                   "Wrong number of columns for Warehouse table schema.");

    return catalog::Schema(warehouse_columns);
  }

  /**
   * @param schema Warehouse table schema
   * @param oid_counter global OID counter to be incremented
   * @return Warehouse primary index schema
   */
  static storage::index::IndexKeySchema BuildWarehousePrimaryIndexSchema(const catalog::Schema &schema,
                                                                         uint32_t *const oid_counter) {
    storage::index::IndexKeySchema warehouse_key_schema;
    warehouse_key_schema.reserve(num_warehouse_primary_index_cols_);

    // Primary Key: W_ID
    warehouse_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                      schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());

    TERRIER_ASSERT(warehouse_key_schema.size() == num_warehouse_primary_index_cols_,
                   "Wrong number of columns for Warehouse primary index schema.");

    return warehouse_key_schema;
  }

  /**
   * @param oid_counter global OID counter to be incremented
   * @return District table schema
   */
  static catalog::Schema BuildDistrictTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> district_columns;
    district_columns.reserve(num_district_table_cols_);

    // 20 unique IDs
    district_columns.emplace_back("D_ID", type::TypeId::TINYINT, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    district_columns.emplace_back("D_W_ID", type::TypeId::TINYINT, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 10
    district_columns.emplace_back("D_NAME", type::TypeId::VARCHAR, 10, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    district_columns.emplace_back("D_STREET_1", type::TypeId::VARCHAR, 20, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    district_columns.emplace_back("D_STREET_2", type::TypeId::VARCHAR, 20, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    district_columns.emplace_back("D_CITY", type::TypeId::VARCHAR, 20, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 2
    district_columns.emplace_back("D_STATE", type::TypeId::VARCHAR, 2, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 9
    district_columns.emplace_back("D_ZIP", type::TypeId::VARCHAR, 9, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(4,4)
    district_columns.emplace_back("D_TAX", type::TypeId::DECIMAL, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(12,2)
    district_columns.emplace_back("D_YTD", type::TypeId::DECIMAL, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 10,000,000 unique IDs
    district_columns.emplace_back("D_NEXT_O_ID", type::TypeId::INTEGER, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(district_columns.size() == num_district_table_cols_,
                   "Wrong number of columns for District table schema.");

    return catalog::Schema(district_columns);
  }

  /**
   * @param schema District table schema
   * @param oid_counter global OID counter to be incremented
   * @return District primary index schema
   */
  static storage::index::IndexKeySchema BuildDistrictPrimaryIndexSchema(const catalog::Schema &schema,
                                                                        uint32_t *const oid_counter) {
    storage::index::IndexKeySchema district_key_schema;
    district_key_schema.reserve(num_district_primary_index_cols_);

    // Primary Key: (D_W_ID, D_ID)
    district_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                     schema.GetColumn(1).GetType(), schema.GetColumn(1).GetNullable());
    district_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                     schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());

    TERRIER_ASSERT(district_key_schema.size() == num_district_primary_index_cols_,
                   "Wrong number of columns for District primary index schema.");

    return district_key_schema;
  }

  /**
   * @param oid_counter global OID counter to be incremented
   * @return Customer table schema
   */
  static catalog::Schema BuildCustomerTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> customer_columns;
    customer_columns.reserve(num_customer_table_cols_);

    // 96,000 unique IDs
    customer_columns.emplace_back("C_ID", type::TypeId::INTEGER, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 20 unique IDs
    customer_columns.emplace_back("C_D_ID", type::TypeId::TINYINT, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    customer_columns.emplace_back("C_W_ID", type::TypeId::TINYINT, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 16
    customer_columns.emplace_back("C_FIRST", type::TypeId::VARCHAR, 16, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 2
    customer_columns.emplace_back("C_MIDDLE", type::TypeId::VARCHAR, 2, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 16
    customer_columns.emplace_back("C_LAST", type::TypeId::VARCHAR, 16, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    customer_columns.emplace_back("C_STREET_1", type::TypeId::VARCHAR, 20, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    customer_columns.emplace_back("C_STREET_2", type::TypeId::VARCHAR, 20, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 20
    customer_columns.emplace_back("C_CITY", type::TypeId::VARCHAR, 20, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 2
    customer_columns.emplace_back("C_STATE", type::TypeId::VARCHAR, 2, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 9
    customer_columns.emplace_back("C_ZIP", type::TypeId::VARCHAR, 9, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 16
    customer_columns.emplace_back("C_PHONE", type::TypeId::VARCHAR, 16, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // date and time
    customer_columns.emplace_back("C_SINCE", type::TypeId::TIMESTAMP, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 2
    customer_columns.emplace_back("C_CREDIT", type::TypeId::VARCHAR, 2, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(12,2)
    customer_columns.emplace_back("C_CREDIT_LIM", type::TypeId::DECIMAL, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(4,4)
    customer_columns.emplace_back("C_DISCOUNT", type::TypeId::DECIMAL, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(12,2)
    customer_columns.emplace_back("C_BALANCE", type::TypeId::DECIMAL, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(12,2)
    customer_columns.emplace_back("C_YTD_PAYMENT", type::TypeId::DECIMAL, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(4)
    customer_columns.emplace_back("C_PAYMENT_CNT", type::TypeId::SMALLINT, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(4)
    customer_columns.emplace_back("C_DELIVERY_CNT", type::TypeId::SMALLINT, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 500
    customer_columns.emplace_back("C_DATA", type::TypeId::VARCHAR, 500, false,
                                  static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(customer_columns.size() == num_customer_table_cols_,
                   "Wrong number of columns for Customer table schema.");

    return catalog::Schema(customer_columns);
  }

  /**
   * @param schema Customer table schema
   * @param oid_counter global OID counter to be incremented
   * @return Customer primary index schema
   */
  static storage::index::IndexKeySchema BuildCustomerPrimaryIndexSchema(const catalog::Schema &schema,
                                                                        uint32_t *const oid_counter) {
    storage::index::IndexKeySchema customer_key_schema;
    customer_key_schema.reserve(num_customer_primary_index_cols_);

    // Primary Key: (C_W_ID, C_D_ID, C_ID)
    customer_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                     schema.GetColumn(2).GetType(), schema.GetColumn(2).GetNullable());
    customer_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                     schema.GetColumn(1).GetType(), schema.GetColumn(1).GetNullable());
    customer_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                     schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());

    TERRIER_ASSERT(customer_key_schema.size() == num_customer_primary_index_cols_,
                   "Wrong number of columns for Customer primary index schema.");

    return customer_key_schema;
  }

  /**
   * @param schema Customer table schema
   * @param oid_counter global OID counter to be incremented
   * @return Customer secondary index schema
   */
  static storage::index::IndexKeySchema BuildCustomerSecondaryIndexSchema(const catalog::Schema &schema,
                                                                          uint32_t *const oid_counter) {
    storage::index::IndexKeySchema customer_secondary_key_schema;
    customer_secondary_key_schema.reserve(num_customer_secondary_index_cols_);

    // C_W_ID, C_D_ID, C_LAST for Order Status and Payment transactions
    customer_secondary_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                               schema.GetColumn(2).GetType(), schema.GetColumn(2).GetNullable());
    customer_secondary_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                               schema.GetColumn(1).GetType(), schema.GetColumn(1).GetNullable());
    customer_secondary_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                               schema.GetColumn(5).GetType(), schema.GetColumn(5).GetNullable(),
                                               schema.GetColumn(5).GetMaxVarlenSize());

    TERRIER_ASSERT(customer_secondary_key_schema.size() == num_customer_secondary_index_cols_,
                   "Wrong number of columns for Customer secondary index schema.");

    return customer_secondary_key_schema;
  }

  /**
   * @param oid_counter global OID counter to be incremented
   * @return History table schema
   */
  static catalog::Schema BuildHistoryTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> history_columns;
    history_columns.reserve(num_history_table_cols_);

    // 96,000 unique IDs
    history_columns.emplace_back("H_C_ID", type::TypeId::INTEGER, false,
                                 static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 20 unique IDs
    history_columns.emplace_back("H_C_D_ID", type::TypeId::TINYINT, false,
                                 static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    history_columns.emplace_back("H_C_W_ID", type::TypeId::TINYINT, false,
                                 static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 20 unique IDs
    history_columns.emplace_back("H_D_ID", type::TypeId::TINYINT, false,
                                 static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    history_columns.emplace_back("H_W_ID", type::TypeId::TINYINT, false,
                                 static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // date and time
    history_columns.emplace_back("H_DATE", type::TypeId::TIMESTAMP, false,
                                 static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(6,2)
    history_columns.emplace_back("H_AMOUNT", type::TypeId::DECIMAL, false,
                                 static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 24
    history_columns.emplace_back("H_DATA", type::TypeId::VARCHAR, 24, false,
                                 static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(history_columns.size() == num_history_table_cols_,
                   "Wrong number of columns for History table schema.");

    return catalog::Schema(history_columns);
  }

  /**
   * @param oid_counter global OID counter to be incremented
   * @return New Order table schema
   */
  static catalog::Schema BuildNewOrderTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> new_order_columns;
    new_order_columns.reserve(num_new_order_table_cols_);

    // 10,000,000 unique IDs
    new_order_columns.emplace_back("NO_O_ID", type::TypeId::INTEGER, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 20 unique IDs
    new_order_columns.emplace_back("NO_D_ID", type::TypeId::TINYINT, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    new_order_columns.emplace_back("NO_W_ID", type::TypeId::TINYINT, false,
                                   static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(new_order_columns.size() == num_new_order_table_cols_,
                   "Wrong number of columns for New Order table schema.");

    return catalog::Schema(new_order_columns);
  }

  /**
   * @param schema New Order table schema
   * @param oid_counter global OID counter to be incremented
   * @return New Order primary index schema
   */
  static storage::index::IndexKeySchema BuildNewOrderPrimaryIndexSchema(const catalog::Schema &schema,
                                                                        uint32_t *const oid_counter) {
    storage::index::IndexKeySchema new_order_key_schema;
    new_order_key_schema.reserve(num_new_order_primary_index_cols_);

    // Primary Key: (NO_W_ID, NO_D_ID, NO_O_ID)
    new_order_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                      schema.GetColumn(2).GetType(), schema.GetColumn(2).GetNullable());
    new_order_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                      schema.GetColumn(1).GetType(), schema.GetColumn(1).GetNullable());
    new_order_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                      schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());

    TERRIER_ASSERT(new_order_key_schema.size() == num_new_order_primary_index_cols_,
                   "Wrong number of columns for New Order primary index schema.");

    return new_order_key_schema;
  }

  /**
   * @param oid_counter global OID counter to be incremented
   * @return Order table schema
   */
  static catalog::Schema BuildOrderTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> order_columns;
    order_columns.reserve(num_order_table_cols_);

    // 10,000,000 unique IDs
    order_columns.emplace_back("O_ID", type::TypeId::INTEGER, false, static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 20 unique IDs
    order_columns.emplace_back("O_D_ID", type::TypeId::TINYINT, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    order_columns.emplace_back("O_W_ID", type::TypeId::TINYINT, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 96,000 unique IDs
    order_columns.emplace_back("O_C_ID", type::TypeId::INTEGER, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // date and time
    order_columns.emplace_back("O_ENTRY_D", type::TypeId::TIMESTAMP, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 10 unique IDs, or null
    order_columns.emplace_back("O_CARRIER_ID", type::TypeId::TINYINT, true,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(2)
    order_columns.emplace_back("O_OL_CNT", type::TypeId::TINYINT, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(1)
    order_columns.emplace_back("O_ALL_LOCAL", type::TypeId::TINYINT, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(order_columns.size() == num_order_table_cols_, "Wrong number of columns for Order table schema.");

    return catalog::Schema(order_columns);
  }

  /**
   * @param schema Order table schema
   * @param oid_counter global OID counter to be incremented
   * @return Order primary index schema
   */
  static storage::index::IndexKeySchema BuildOrderPrimaryIndexSchema(const catalog::Schema &schema,
                                                                     uint32_t *const oid_counter) {
    storage::index::IndexKeySchema order_key_schema;
    order_key_schema.reserve(num_order_primary_index_cols_);

    // Primary Key: (O_W_ID, O_D_ID, O_ID)
    order_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                  schema.GetColumn(2).GetType(), schema.GetColumn(2).GetNullable());
    order_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                  schema.GetColumn(1).GetType(), schema.GetColumn(1).GetNullable());
    order_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                  schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());

    TERRIER_ASSERT(order_key_schema.size() == num_order_primary_index_cols_,
                   "Wrong number of columns for Order primary index schema.");

    return order_key_schema;
  }

  /**
   * @param schema Order table schema
   * @param oid_counter global OID counter to be incremented
   * @return Order secondary index schema
   */
  static storage::index::IndexKeySchema BuildOrderSecondaryIndexSchema(const catalog::Schema &schema,
                                                                       uint32_t *const oid_counter) {
    storage::index::IndexKeySchema order_secondary_key_schema;
    order_secondary_key_schema.reserve(num_order_secondary_index_cols_);

    // O_W_ID, O_D_ID, O_C_ID, O_ID for Order Status transaction
    order_secondary_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                            schema.GetColumn(2).GetType(), schema.GetColumn(2).GetNullable());
    order_secondary_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                            schema.GetColumn(1).GetType(), schema.GetColumn(1).GetNullable());
    order_secondary_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                            schema.GetColumn(3).GetType(), schema.GetColumn(3).GetNullable());
    order_secondary_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                            schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());

    TERRIER_ASSERT(order_secondary_key_schema.size() == num_order_secondary_index_cols_,
                   "Wrong number of columns for Order secondary index schema.");

    return order_secondary_key_schema;
  }

  /**
   * @param oid_counter global OID counter to be incremented
   * @return Order Line table schema
   */
  static catalog::Schema BuildOrderLineTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> order_line_columns;
    order_line_columns.reserve(num_order_line_table_cols_);

    // 10,000,000 unique IDs
    order_line_columns.emplace_back("OL_O_ID", type::TypeId::INTEGER, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 20 unique IDs
    order_line_columns.emplace_back("OL_D_ID", type::TypeId::TINYINT, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    order_line_columns.emplace_back("OL_W_ID", type::TypeId::TINYINT, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 15 unique IDs
    order_line_columns.emplace_back("OL_NUMBER", type::TypeId::TINYINT, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 200,000 unique IDs
    order_line_columns.emplace_back("OL_I_ID", type::TypeId::INTEGER, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    order_line_columns.emplace_back("OL_SUPPLY_W_ID", type::TypeId::TINYINT, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // date and time, or null
    order_line_columns.emplace_back("OL_DELIVERY_D", type::TypeId::TIMESTAMP, true,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(2)
    order_line_columns.emplace_back("OL_QUANTITY", type::TypeId::TINYINT, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(6,2)
    order_line_columns.emplace_back("OL_AMOUNT", type::TypeId::DECIMAL, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    order_line_columns.emplace_back("OL_DIST_INFO", type::TypeId::VARCHAR, 24, false,
                                    static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(order_line_columns.size() == num_order_line_table_cols_,
                   "Wrong number of columns for Order Line table schema.");

    return catalog::Schema(order_line_columns);
  }

  /**
   * @param schema Order Line table schema
   * @param oid_counter global OID counter to be incremented
   * @return Order Line primary index schema
   */
  static storage::index::IndexKeySchema BuildOrderLinePrimaryIndexSchema(const catalog::Schema &schema,
                                                                         uint32_t *const oid_counter) {
    storage::index::IndexKeySchema order_line_key_schema;
    order_line_key_schema.reserve(num_order_line_primary_index_cols_);

    // Primary Key: (OL_W_ID, OL_D_ID, OL_O_ID, OL_NUMBER)
    order_line_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                       schema.GetColumn(2).GetType(), schema.GetColumn(2).GetNullable());
    order_line_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                       schema.GetColumn(1).GetType(), schema.GetColumn(1).GetNullable());
    order_line_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                       schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());
    order_line_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                       schema.GetColumn(3).GetType(), schema.GetColumn(3).GetNullable());

    TERRIER_ASSERT(order_line_key_schema.size() == num_order_line_primary_index_cols_,
                   "Wrong number of columns for Order Line key schema.");

    return order_line_key_schema;
  }

  /**
   * @param oid_counter global OID counter to be incremented
   * @return Item table schema
   */
  static catalog::Schema BuildItemTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> item_columns;
    item_columns.reserve(num_item_table_cols_);

    // 200,000 unique IDs
    item_columns.emplace_back("I_ID", type::TypeId::INTEGER, false, static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 200,000 unique IDs
    item_columns.emplace_back("I_IM_ID", type::TypeId::INTEGER, false,
                              static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 24
    item_columns.emplace_back("I_NAME", type::TypeId::VARCHAR, 24, false,
                              static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(5,2)
    item_columns.emplace_back("I_PRICE", type::TypeId::DECIMAL, false,
                              static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 50
    item_columns.emplace_back("I_DATA", type::TypeId::VARCHAR, 50, false,
                              static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(item_columns.size() == num_item_table_cols_, "Wrong number of columns for Item table schema.");

    return catalog::Schema(item_columns);
  }

  /**
   * @param schema Item table schema
   * @param oid_counter global OID counter to be incremented
   * @return Item primary index schema
   */
  static storage::index::IndexKeySchema BuildItemPrimaryIndexSchema(const catalog::Schema &schema,
                                                                    uint32_t *const oid_counter) {
    storage::index::IndexKeySchema item_key_schema;
    item_key_schema.reserve(num_item_primary_index_cols_);

    // Primary Key: I_ID
    item_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                 schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());

    TERRIER_ASSERT(item_key_schema.size() == num_item_primary_index_cols_,
                   "Wrong number of columns for Item primary index schema.");

    return item_key_schema;
  }

  /**
   * @param oid_counter global OID counter to be incremented
   * @return Stock table schema
   */
  static catalog::Schema BuildStockTableSchema(uint32_t *const oid_counter) {
    std::vector<catalog::Schema::Column> stock_columns;
    stock_columns.reserve(num_stock_table_cols_);

    // 200,000 unique IDs
    stock_columns.emplace_back("S_I_ID", type::TypeId::INTEGER, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // 2*W unique IDs
    stock_columns.emplace_back("S_W_ID", type::TypeId::TINYINT, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // signed numeric(4)
    stock_columns.emplace_back("S_QUANTITY", type::TypeId::SMALLINT, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_01", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_02", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_03", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_04", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_05", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_06", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_07", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_08", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_09", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // fixed text, size 24
    stock_columns.emplace_back("S_DIST_10", type::TypeId::VARCHAR, 24, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(8)
    stock_columns.emplace_back("S_YTD", type::TypeId::INTEGER, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(4)
    stock_columns.emplace_back("S_ORDER_CNT", type::TypeId::SMALLINT, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // numeric(4)
    stock_columns.emplace_back("S_REMOTE_CNT", type::TypeId::SMALLINT, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));
    // variable text, size 50
    stock_columns.emplace_back("S_DATA", type::TypeId::VARCHAR, 50, false,
                               static_cast<catalog::col_oid_t>(++(*oid_counter)));

    TERRIER_ASSERT(stock_columns.size() == num_stock_table_cols_, "Wrong number of columns for Stock table schema.");

    return catalog::Schema(stock_columns);
  }

  /**
   * @param schema Stock table schema
   * @param oid_counter global OID counter to be incremented
   * @return Stock primary index schema
   */
  static storage::index::IndexKeySchema BuildStockPrimaryIndexSchema(const catalog::Schema &schema,
                                                                     uint32_t *const oid_counter) {
    storage::index::IndexKeySchema stock_key_schema;
    stock_key_schema.reserve(num_stock_primary_index_cols_);

    // Primary Key: (S_W_ID, S_I_ID)
    stock_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                  schema.GetColumn(1).GetType(), schema.GetColumn(1).GetNullable());
    stock_key_schema.emplace_back(static_cast<catalog::indexkeycol_oid_t>(++(*oid_counter)),
                                  schema.GetColumn(0).GetType(), schema.GetColumn(0).GetNullable());

    TERRIER_ASSERT(stock_key_schema.size() == num_stock_primary_index_cols_,
                   "Wrong number of columns for Stock primary index schema.");

    return stock_key_schema;
  }

 private:
  // The values below are just to sanity check the schema functions
  static constexpr uint8_t num_warehouse_table_cols_ = 9;
  static constexpr uint8_t num_district_table_cols_ = 11;
  static constexpr uint8_t num_customer_table_cols_ = 21;
  static constexpr uint8_t num_history_table_cols_ = 8;
  static constexpr uint8_t num_new_order_table_cols_ = 3;
  static constexpr uint8_t num_order_table_cols_ = 8;
  static constexpr uint8_t num_order_line_table_cols_ = 10;
  static constexpr uint8_t num_item_table_cols_ = 5;
  static constexpr uint8_t num_stock_table_cols_ = 17;

  static constexpr uint8_t num_warehouse_primary_index_cols_ = 1;
  static constexpr uint8_t num_district_primary_index_cols_ = 2;
  static constexpr uint8_t num_customer_primary_index_cols_ = 3;
  static constexpr uint8_t num_customer_secondary_index_cols_ = 3;
  static constexpr uint8_t num_new_order_primary_index_cols_ = 3;
  static constexpr uint8_t num_order_primary_index_cols_ = 3;
  static constexpr uint8_t num_order_secondary_index_cols_ = 4;
  static constexpr uint8_t num_order_line_primary_index_cols_ = 4;
  static constexpr uint8_t num_item_primary_index_cols_ = 1;
  static constexpr uint8_t num_stock_primary_index_cols_ = 2;
};

}  // namespace terrier::tpcc
