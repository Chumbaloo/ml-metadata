/* Copyright 2020 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "ml_metadata/tools/mlmd_bench/util.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "ml_metadata/metadata_store/metadata_store.h"
#include "ml_metadata/metadata_store/metadata_store_factory.h"
#include "ml_metadata/proto/metadata_store.pb.h"
#include "ml_metadata/proto/metadata_store_service.pb.h"
#include "tensorflow/core/lib/core/status_test_util.h"

namespace ml_metadata {
namespace {

constexpr int kNumberOfInsertedArtifactTypes = 51;
constexpr int kNumberOfInsertedExecutionTypes = 52;
constexpr int kNumberOfInsertedContextTypes = 53;

constexpr int kNumberOfInsertedArtifacts = 101;
constexpr int kNumberOfInsertedExecutions = 102;
constexpr int kNumberOfInsertedContexts = 103;

// Tests InsertTypesInDb().
TEST(UtilInsertTest, InsertTypesTest) {
  std::unique_ptr<MetadataStore> store;
  ConnectionConfig mlmd_config;
  // Uses a fake in-memory SQLite database for testing.
  mlmd_config.mutable_fake_database();
  TF_ASSERT_OK(CreateMetadataStore(mlmd_config, &store));
  TF_ASSERT_OK(InsertTypesInDb(
      /*num_artifact_types=*/kNumberOfInsertedArtifactTypes,
      /*num_execution_types=*/kNumberOfInsertedExecutionTypes,
      /*num_context_types=*/kNumberOfInsertedContextTypes, *store));

  GetArtifactTypesResponse get_artifact_types_response;
  TF_ASSERT_OK(store->GetArtifactTypes(
      /*request=*/{}, &get_artifact_types_response));
  GetExecutionTypesResponse get_execution_types_response;
  TF_ASSERT_OK(store->GetExecutionTypes(
      /*request=*/{}, &get_execution_types_response));
  GetContextTypesResponse get_context_types_response;
  TF_ASSERT_OK(store->GetContextTypes(
      /*request=*/{}, &get_context_types_response));

  EXPECT_THAT(get_artifact_types_response.artifact_types(),
              ::testing::SizeIs(kNumberOfInsertedArtifactTypes));
  EXPECT_THAT(get_execution_types_response.execution_types(),
              ::testing::SizeIs(kNumberOfInsertedExecutionTypes));
  EXPECT_THAT(get_context_types_response.context_types(),
              ::testing::SizeIs(kNumberOfInsertedContextTypes));
}

// Tests InsertNodesInDb().
TEST(UtilInsertTest, InsertNodesTest) {
  std::unique_ptr<MetadataStore> store;
  ConnectionConfig mlmd_config;
  // Uses a fake in-memory SQLite database for testing.
  mlmd_config.mutable_fake_database();
  TF_ASSERT_OK(CreateMetadataStore(mlmd_config, &store));
  TF_ASSERT_OK(InsertTypesInDb(
      /*num_artifact_types=*/kNumberOfInsertedArtifactTypes,
      /*num_execution_types=*/kNumberOfInsertedExecutionTypes,
      /*num_context_types=*/kNumberOfInsertedContextTypes, *store));
  TF_ASSERT_OK(InsertNodesInDb(
      /*num_artifact_nodes=*/kNumberOfInsertedArtifacts,
      /*num_execution_nodes=*/kNumberOfInsertedExecutions,
      /*num_context_nodes=*/kNumberOfInsertedContexts, *store));

  GetArtifactsResponse get_artifacts_response;
  TF_ASSERT_OK(store->GetArtifacts(
      /*request=*/{}, &get_artifacts_response));
  GetExecutionsResponse get_executions_response;
  TF_ASSERT_OK(store->GetExecutions(
      /*request=*/{}, &get_executions_response));
  GetContextsResponse get_contexts_response;
  TF_ASSERT_OK(store->GetContexts(
      /*request=*/{}, &get_contexts_response));

  EXPECT_THAT(get_artifacts_response.artifacts(),
              ::testing::SizeIs(kNumberOfInsertedArtifacts));
  EXPECT_THAT(get_executions_response.executions(),
              ::testing::SizeIs(kNumberOfInsertedExecutions));
  EXPECT_THAT(get_contexts_response.contexts(),
              ::testing::SizeIs(kNumberOfInsertedContexts));
}

// Tests GetExistingTypes() with FillTypesConfig as input.
TEST(UtilGetTest, GetTypesWithFillTypesConfigTest) {
  std::unique_ptr<MetadataStore> store;
  ConnectionConfig mlmd_config;
  // Uses a fake in-memory SQLite database for testing.
  mlmd_config.mutable_fake_database();
  TF_ASSERT_OK(CreateMetadataStore(mlmd_config, &store));
  TF_ASSERT_OK(InsertTypesInDb(
      /*num_artifact_types=*/kNumberOfInsertedArtifactTypes,
      /*num_execution_types=*/kNumberOfInsertedExecutionTypes,
      /*num_context_types=*/kNumberOfInsertedContextTypes, *store));

  {
    std::vector<Type> exisiting_types;
    FillTypesConfig fill_types_config;
    fill_types_config.set_specification(FillTypesConfig::ARTIFACT_TYPE);
    TF_ASSERT_OK(GetExistingTypes(fill_types_config, *store, exisiting_types));
    EXPECT_THAT(exisiting_types,
                ::testing::SizeIs(kNumberOfInsertedArtifactTypes));
  }

  {
    std::vector<Type> exisiting_types;
    FillTypesConfig fill_types_config;
    fill_types_config.set_specification(FillTypesConfig::EXECUTION_TYPE);
    TF_ASSERT_OK(GetExistingTypes(fill_types_config, *store, exisiting_types));
    EXPECT_THAT(exisiting_types,
                ::testing::SizeIs(kNumberOfInsertedExecutionTypes));
  }

  {
    std::vector<Type> exisiting_types;
    FillTypesConfig fill_types_config;
    fill_types_config.set_specification(FillTypesConfig::CONTEXT_TYPE);
    TF_ASSERT_OK(GetExistingTypes(fill_types_config, *store, exisiting_types));
    EXPECT_THAT(exisiting_types,
                ::testing::SizeIs(kNumberOfInsertedContextTypes));
  }
}

// Tests GetExistingTypes() with FillNodesConfig as input.
TEST(UtilGetTest, GetTypesWithFillNodesConfigTest) {
  std::unique_ptr<MetadataStore> store;
  ConnectionConfig mlmd_config;
  // Uses a fake in-memory SQLite database for testing.
  mlmd_config.mutable_fake_database();
  TF_ASSERT_OK(CreateMetadataStore(mlmd_config, &store));
  TF_ASSERT_OK(InsertTypesInDb(
      /*num_artifact_types=*/kNumberOfInsertedArtifactTypes,
      /*num_execution_types=*/kNumberOfInsertedExecutionTypes,
      /*num_context_types=*/kNumberOfInsertedContextTypes, *store));

  {
    std::vector<Type> exisiting_types;
    FillNodesConfig fill_nodes_config;
    fill_nodes_config.set_specification(FillNodesConfig::ARTIFACT);
    TF_ASSERT_OK(GetExistingTypes(fill_nodes_config, *store, exisiting_types));
    EXPECT_THAT(exisiting_types,
                ::testing::SizeIs(kNumberOfInsertedArtifactTypes));
  }

  {
    std::vector<Type> exisiting_types;
    FillNodesConfig fill_nodes_config;
    fill_nodes_config.set_specification(FillNodesConfig::EXECUTION);
    TF_ASSERT_OK(GetExistingTypes(fill_nodes_config, *store, exisiting_types));
    EXPECT_THAT(exisiting_types,
                ::testing::SizeIs(kNumberOfInsertedExecutionTypes));
  }

  {
    std::vector<Type> exisiting_types;
    FillNodesConfig fill_nodes_config;
    fill_nodes_config.set_specification(FillNodesConfig::CONTEXT);
    TF_ASSERT_OK(GetExistingTypes(fill_nodes_config, *store, exisiting_types));
    EXPECT_THAT(exisiting_types,
                ::testing::SizeIs(kNumberOfInsertedContextTypes));
  }
}

// Tests GetExistingNodes() with FillNodesConfig as input.
TEST(UtilGetTest, GetNodesWithFillNodesConfigTest) {
  std::unique_ptr<MetadataStore> store;
  ConnectionConfig mlmd_config;
  // Uses a fake in-memory SQLite database for testing.
  mlmd_config.mutable_fake_database();
  TF_ASSERT_OK(CreateMetadataStore(mlmd_config, &store));
  TF_ASSERT_OK(InsertTypesInDb(
      /*num_artifact_types=*/kNumberOfInsertedArtifactTypes,
      /*num_execution_types=*/kNumberOfInsertedExecutionTypes,
      /*num_context_types=*/kNumberOfInsertedContextTypes, *store));
  TF_ASSERT_OK(InsertNodesInDb(
      /*num_artifact_nodes=*/kNumberOfInsertedArtifacts,
      /*num_execution_nodes=*/kNumberOfInsertedExecutions,
      /*num_context_nodes=*/kNumberOfInsertedContexts, *store));

  {
    std::vector<Node> exisiting_nodes;
    FillNodesConfig fill_nodes_config;
    fill_nodes_config.set_specification(FillNodesConfig::ARTIFACT);
    TF_ASSERT_OK(GetExistingNodes(fill_nodes_config, *store, exisiting_nodes));
    EXPECT_THAT(exisiting_nodes, ::testing::SizeIs(kNumberOfInsertedArtifacts));
  }

  {
    std::vector<Node> exisiting_nodes;
    FillNodesConfig fill_nodes_config;
    fill_nodes_config.set_specification(FillNodesConfig::EXECUTION);
    TF_ASSERT_OK(GetExistingNodes(fill_nodes_config, *store, exisiting_nodes));
    EXPECT_THAT(exisiting_nodes,
                ::testing::SizeIs(kNumberOfInsertedExecutions));
  }

  {
    std::vector<Node> exisiting_nodes;
    FillNodesConfig fill_nodes_config;
    fill_nodes_config.set_specification(FillNodesConfig::CONTEXT);
    TF_ASSERT_OK(GetExistingNodes(fill_nodes_config, *store, exisiting_nodes));
    EXPECT_THAT(exisiting_nodes, ::testing::SizeIs(kNumberOfInsertedContexts));
  }
}

// Tests GetExistingNodes() with FillContextEdgesConfig as input.
TEST(UtilGetTest, GetNodesWithFillContextEdgesConfigTest) {
  std::unique_ptr<MetadataStore> store;
  ConnectionConfig mlmd_config;
  // Uses a fake in-memory SQLite database for testing.
  mlmd_config.mutable_fake_database();
  TF_ASSERT_OK(CreateMetadataStore(mlmd_config, &store));
  TF_ASSERT_OK(InsertTypesInDb(
      /*num_artifact_types=*/kNumberOfInsertedArtifactTypes,
      /*num_execution_types=*/kNumberOfInsertedExecutionTypes,
      /*num_context_types=*/kNumberOfInsertedContextTypes, *store));
  TF_ASSERT_OK(InsertNodesInDb(
      /*num_artifact_nodes=*/kNumberOfInsertedArtifacts,
      /*num_execution_nodes=*/kNumberOfInsertedExecutions,
      /*num_context_nodes=*/kNumberOfInsertedContexts, *store));

  {
    std::vector<Node> existing_non_context_nodes;
    std::vector<Node> existing_context_nodes;
    FillContextEdgesConfig fill_context_edges_config;
    fill_context_edges_config.set_specification(
        FillContextEdgesConfig::ATTRIBUTION);
    TF_ASSERT_OK(GetExistingNodes(fill_context_edges_config, *store,
                                  existing_non_context_nodes,
                                  existing_context_nodes));
    EXPECT_EQ(kNumberOfInsertedArtifacts, existing_non_context_nodes.size());
    EXPECT_EQ(kNumberOfInsertedContexts, existing_context_nodes.size());
  }

  {
    std::vector<Node> existing_non_context_nodes;
    std::vector<Node> existing_context_nodes;
    FillContextEdgesConfig fill_context_edges_config;
    fill_context_edges_config.set_specification(
        FillContextEdgesConfig::ASSOCIATION);
    TF_ASSERT_OK(GetExistingNodes(fill_context_edges_config, *store,
                                  existing_non_context_nodes,
                                  existing_context_nodes));
    EXPECT_EQ(kNumberOfInsertedExecutions, existing_non_context_nodes.size());
    EXPECT_EQ(kNumberOfInsertedContexts, existing_context_nodes.size());
  }
}

}  // namespace
}  // namespace ml_metadata
