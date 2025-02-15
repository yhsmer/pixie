# Demo applications

These demo applications are packaged to allow `px deploy` to access them. The `manifest.json`
file describes the demo scenarios.

## General Notes

- For best results when running the continuous profiler, all Java applications should be run with `-XX:+PreserveFramePointer`.

## Instructions for adding a new demo

1. Add a folder containing the demo yaml and license file.
2. Add the demo to the `manifest.json` file.
3. Test the CLI:
    1. (Optional) Update the GCS bucket in the `demos/BUILD.bazel` demo_upload step. Set the artifacts URL appropriately.

        ```shell
        export DEV_DEMO_ARTIFACTS_URL=https://storage.googleapis.com/pl-infra-dev-artifacts/dev-demo-apps
        ```

    2. Upload the demo artifacts to the dev bucket:

        ```shell
        bazel run //demos:upload_dev_demo
        ```

    3. Test the CLI with the dev artifacts:

        ```shell
        px demo list --artifacts "${DEV_DEMO_ARTIFACTS_URL}"
        px demo deploy <DEMO_NAME> --artifacts "${DEV_DEMO_ARTIFACTS_URL}"
        ```

4. After your PR is merged, upload the demo artifacts to the prod bucket:

    ```shell
    bazel run //demos:upload_prod_demo
    ```

## Updating the `px-kafka` demo

1. Clone `https://github.com/pixie-io/microservice-kafka` and switch to the `pixie` branch.

2. (optional) Build the container image & update the individual yaml files.

3. Build a single yaml file for the demo:

    ```shell
    kustomize build . >  kafka.yaml
    ```

4. Copy the yaml file to `pixie/demos/kafka`.

## Updating the `px-finagle` demo

1. Clone `https://github.com/pixie-io/finagle-helloworld`

2. (optional) Build the container images & update the individual yaml files.

3. Build a single yaml file for the demo:

    ```shell
    kustomize . > finagle.yaml
    ```

4. Copy the yaml file to `pixie/demos/finagle`.

## Updating the `px-online-boutique` demo

Our custom `adservice` image includes the `-XX:+PreserveFramePointer` Java option. To build our custom `adservice` image:

1. Clone `https://github.com/pixie-io/microservices-demo` and switch to the `pixie` branch.

2. Run the following commands:

    ```shell
    cd src/adservice
    docker build -t gcr.io/pixie-prod/demos/microservices-demo-app/adservice:1.0 .
    docker push gcr.io/pixie-prod/demos/microservices-demo-app/adservice:1.0
    ```
