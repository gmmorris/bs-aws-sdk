module MockAws = {
  type upload = string => unit;
  [@bs.deriving abstract]
  type s3Instance = {upload};
  type s3 = unit => s3Instance;
  [@bs.deriving abstract]
  type aws = {_S3: s3};
};

let onUploadToS3 = onUpload =>
  MockAws.(aws(~_S3=() => s3Instance(~upload=onUpload)));