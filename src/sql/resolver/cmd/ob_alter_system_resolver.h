/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef OCEANBASE_RESOLVER_CMD_OB_ALTER_SYSTEM_RESOLVER_
#define OCEANBASE_RESOLVER_CMD_OB_ALTER_SYSTEM_RESOLVER_

#include "sql/resolver/cmd/ob_system_cmd_resolver.h"
#include "sql/session/ob_sql_session_info.h" // ObSqlSessionInfo
#include "share/ls/ob_ls_i_life_manager.h" //OB_LS_MAX_SCN_VALUE

namespace oceanbase
{
namespace common
{
class ObAddr;
}
namespace sql
{
class ObSystemCmdStmt;
class ObFreezeStmt;
class ObAlterSystemResolverUtil
{
public:
  static int sanity_check(const ParseNode *parse_tree, ObItemType item_type);

  // resolve opt_ip_port
  static int resolve_server(const ParseNode *parse_tree, common::ObAddr &server);
  // resolve server string (value part of opt_ip_port)
  static int resolve_server_value(const ParseNode *parse_tree, common::ObAddr &server);
  // resolve opt_zone_desc
  static int resolve_zone(const ParseNode *parse_tree, common::ObZone &zone);
  // resolve opt_tenant_name
  static int resolve_tenant(const ParseNode *parse_tree,
                            common::ObFixedLengthString < common::OB_MAX_TENANT_NAME_LENGTH + 1 > &tenant_name);
  static int resolve_tenant_id(const ParseNode *parse_tree, uint64_t &tenant_id);
  static int resolve_ls_id(const ParseNode *parse_tree, int64_t &ls_id);

  static int resolve_replica_type(const ParseNode *parse_tree,
                                  common::ObReplicaType &replica_type);
  static int resolve_memstore_percent(const ParseNode *parse_tree,
                                      ObReplicaProperty &replica_property);
  static int resolve_string(const ParseNode *parse_tree, common::ObString &string);
  static int resolve_relation_name(const ParseNode *parse_tree, common::ObString &string);
  static int resolve_force_opt(const ParseNode *parse_tree, bool &force_cmd);
  // resolve opt_server_or_zone
  template <typename RPC_ARG>
  static int resolve_server_or_zone(const ParseNode *parse_tree, RPC_ARG &arg);
  // resolve opt_backup_tenant_list
  static int get_tenant_ids(const ParseNode &t_node, common::ObIArray<uint64_t> &tenant_ids);


  static int resolve_tablet_id(const ParseNode *opt_tablet_id, ObTabletID &tablet_id);
  static int resolve_tenant(const ParseNode &tenants_node, 
                            const uint64_t tenant_id,
                            common::ObSArray<uint64_t> &tenant_ids,
                            bool &affect_all);
};

#define DEF_SIMPLE_CMD_RESOLVER(name)                                   \
  class name : public ObSystemCmdResolver                               \
  {                                                                     \
  public:                                                               \
    name(ObResolverParams &params) : ObSystemCmdResolver(params) {}     \
    virtual ~name() {}                                                  \
    virtual int resolve(const ParseNode &parse_tree);                   \
  };

DEF_SIMPLE_CMD_RESOLVER(ObFlushCacheResolver);

DEF_SIMPLE_CMD_RESOLVER(ObFlushKVCacheResolver);

DEF_SIMPLE_CMD_RESOLVER(ObFlushIlogCacheResolver);

DEF_SIMPLE_CMD_RESOLVER(ObFlushDagWarningsResolver);

DEF_SIMPLE_CMD_RESOLVER(ObAdminServerResolver);

DEF_SIMPLE_CMD_RESOLVER(ObAdminZoneResolver);

DEF_SIMPLE_CMD_RESOLVER(ObSwitchReplicaRoleResolver);

DEF_SIMPLE_CMD_RESOLVER(ObReportReplicaResolver);

DEF_SIMPLE_CMD_RESOLVER(ObRecycleReplicaResolver);

DEF_SIMPLE_CMD_RESOLVER(ObAdminMergeResolver);

DEF_SIMPLE_CMD_RESOLVER(ObAdminRecoveryResolver);

DEF_SIMPLE_CMD_RESOLVER(ObClearRootTableResolver);

DEF_SIMPLE_CMD_RESOLVER(ObRefreshSchemaResolver);

DEF_SIMPLE_CMD_RESOLVER(ObRefreshMemStatResolver);

DEF_SIMPLE_CMD_RESOLVER(ObWashMemFragmentationResolver);

DEF_SIMPLE_CMD_RESOLVER(ObRefreshIOCalibrationResolver);

DEF_SIMPLE_CMD_RESOLVER(ObSetTPResolver);

DEF_SIMPLE_CMD_RESOLVER(ObClearLocationCacheResolver);

DEF_SIMPLE_CMD_RESOLVER(ObReloadGtsResolver);

DEF_SIMPLE_CMD_RESOLVER(ObReloadUnitResolver);

DEF_SIMPLE_CMD_RESOLVER(ObReloadServerResolver);

DEF_SIMPLE_CMD_RESOLVER(ObReloadZoneResolver);

DEF_SIMPLE_CMD_RESOLVER(ObClearMergeErrorResolver);

DEF_SIMPLE_CMD_RESOLVER(ObAddArbitrationServiceResolver);
DEF_SIMPLE_CMD_RESOLVER(ObRemoveArbitrationServiceResolver);
DEF_SIMPLE_CMD_RESOLVER(ObReplaceArbitrationServiceResolver);

DEF_SIMPLE_CMD_RESOLVER(ObMigrateUnitResolver);

DEF_SIMPLE_CMD_RESOLVER(ObUpgradeVirtualSchemaResolver);

DEF_SIMPLE_CMD_RESOLVER(ObRunJobResolver);
DEF_SIMPLE_CMD_RESOLVER(ObRunUpgradeJobResolver);
DEF_SIMPLE_CMD_RESOLVER(ObStopUpgradeJobResolver);

DEF_SIMPLE_CMD_RESOLVER(ObSwitchRSRoleResolver);

DEF_SIMPLE_CMD_RESOLVER(ObAdminUpgradeCmdResolver);
DEF_SIMPLE_CMD_RESOLVER(ObAdminRollingUpgradeCmdResolver);

DEF_SIMPLE_CMD_RESOLVER(ObRefreshTimeZoneInfoResolver);

DEF_SIMPLE_CMD_RESOLVER(ObCancelTaskResolver);

DEF_SIMPLE_CMD_RESOLVER(ObSetDiskValidResolver);

DEF_SIMPLE_CMD_RESOLVER(ObClearBalanceTaskResolver);
DEF_SIMPLE_CMD_RESOLVER(ObChangeTenantResolver);

DEF_SIMPLE_CMD_RESOLVER(ObDropTempTableResolver);
DEF_SIMPLE_CMD_RESOLVER(ObRefreshTempTableResolver);

DEF_SIMPLE_CMD_RESOLVER(ObAlterDiskgroupAddDiskResolver);
DEF_SIMPLE_CMD_RESOLVER(ObAlterDiskgroupDropDiskResolver);
DEF_SIMPLE_CMD_RESOLVER(ObArchiveLogResolver);

DEF_SIMPLE_CMD_RESOLVER(ObBackupArchiveLogResolver);
DEF_SIMPLE_CMD_RESOLVER(ObBackupSetEncryptionResolver);
DEF_SIMPLE_CMD_RESOLVER(ObBackupSetDecryptionResolver);
DEF_SIMPLE_CMD_RESOLVER(ObAddRestoreSourceResolver);
DEF_SIMPLE_CMD_RESOLVER(ObClearRestoreSourceResolver);
DEF_SIMPLE_CMD_RESOLVER(ObCheckpointSlogResolver);

int resolve_restore_until(const ParseNode &time_node,
                          const ObSQLSessionInfo *session_info,
                          share::SCN &recovery_until_scn,
                          bool &with_restore_scn);

class ObPhysicalRestoreTenantResolver : public ObSystemCmdResolver
{
  public:
    ObPhysicalRestoreTenantResolver(ObResolverParams &params) : ObSystemCmdResolver(params) {}
    virtual ~ObPhysicalRestoreTenantResolver() {}
    virtual int resolve(const ParseNode &parse_tree);
  private:
    int resolve_decryption_passwd(obrpc::ObPhysicalRestoreTenantArg &arg);
    int resolve_restore_source_array(obrpc::ObPhysicalRestoreTenantArg &arg);
};

class ObRecoverTenantResolver : public ObSystemCmdResolver
{
  public:
    ObRecoverTenantResolver(ObResolverParams &params) : ObSystemCmdResolver(params) {}
    virtual ~ObRecoverTenantResolver() {}
    virtual int resolve(const ParseNode &parse_tree);
  private:
};

class ObAlterSystemSetResolver : public ObSystemCmdResolver
{
public:
  ObAlterSystemSetResolver(ObResolverParams &params) : ObSystemCmdResolver(params) {}
  virtual ~ObAlterSystemSetResolver() {}
  virtual int resolve(const ParseNode &parse_tree);
private:
  int check_param_valid(int64_t tenant_id,
      const common::ObString &name_node, const common::ObString &value_node);
};

class ObSetConfigResolver : public ObSystemCmdResolver
{
public:
  ObSetConfigResolver(ObResolverParams &params) : ObSystemCmdResolver(params) {}
  virtual ~ObSetConfigResolver() {}
  virtual int resolve(const ParseNode &parse_tree);
private:
  int check_param_valid(int64_t tenant_id,
      const common::ObString &name_node, const common::ObString &value_node);
};

class ObFreezeResolver : public ObSystemCmdResolver {
public:
  ObFreezeResolver(ObResolverParams &params) : ObSystemCmdResolver(params) {}
  virtual ~ObFreezeResolver() {}
  virtual int resolve(const ParseNode &parse_tree);
private:
  int resolve_major_freeze_(ObFreezeStmt *freeze_stmt, ParseNode *opt_tenant_list_v2);
  int resolve_minor_freeze_(ObFreezeStmt *freeze_stmt,
                            ParseNode *opt_tenant_list_or_ls_or_tablet_id,
                            ParseNode *opt_server_list,
                            ParseNode *opt_zone_desc);

  int resolve_tenant_ls_tablet_(ObFreezeStmt *freeze_stmt, ParseNode *opt_tenant_list_or_ls_or_tablet_id);
  int resolve_server_list_(ObFreezeStmt *freeze_stmt, ParseNode *opt_server_list);

};

DEF_SIMPLE_CMD_RESOLVER(ObBackupDatabaseResolver);
DEF_SIMPLE_CMD_RESOLVER(ObBackupManageResolver);
DEF_SIMPLE_CMD_RESOLVER(ObBackupCleanResolver);
DEF_SIMPLE_CMD_RESOLVER(ObDeletePolicyResolver);
DEF_SIMPLE_CMD_RESOLVER(ObBackupKeyResolver);
DEF_SIMPLE_CMD_RESOLVER(ObEnableSqlThrottleResolver);
DEF_SIMPLE_CMD_RESOLVER(ObDisableSqlThrottleResolver);
DEF_SIMPLE_CMD_RESOLVER(ObSetRegionBandwidthResolver);
DEF_SIMPLE_CMD_RESOLVER(ObCancelRestoreResolver);

#undef DEF_SIMPLE_CMD_RESOLVER

} // end namespace sql
} // end namespace oceanbase
#endif // OCEANBASE_RESOLVER_CMD_OB_ALTER_SYSTEM_RESOLVER_
